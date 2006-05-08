/* ============================================================================
 * << File: video.c >>
 * -------------------
 *  Authors: Diego Essaya <dessaya@fi.uba.ar>
 *  Date: Mon Feb 28 20:35:10 ART 2005
 *
 *  Description:
 *    Interface for video playing routines. Implementation based on GStreamer.
 * ============================================================================
 * ============================================================================
 *
 * ChangeLog: View Changelog
 *
 * ============================================================================
 * Copyright (C) 2003, 2004 Diego Essaya, Emiliano Castagnari
 * 
 * This file is part of Ixion.
 * 
 * Ixion is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Ixion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 * ============================================================================
 */

#include "video.h"

#include <gst/xoverlay/xoverlay.h>

/* Public elements: */
static GstElement *pipeline, *vthread, *athread, *asink, *xvsink, *vdrop, *src;
static GstPad *vqueuesink, *aqueuesink, *apeer;

/* Status variables: */
static int audio_disabled;  /* 1 if audio should not be processed */
static int status;
static int video_width;
static int video_height;

/* Mutex for the status variable: */
static GMutex *status_mutex;

/* This callback will be called when GStreamer finds some info about the
 * video. In this case we want the width & height. */
static void cb_notify_caps(GObject *obj, 
		GParamSpec *pspec, gpointer data)
{
	GstPad *pad = GST_PAD(obj);
	GstCaps *caps;
	GstStructure *str;
	gint width, height;

	if (!(caps = gst_pad_get_caps (pad))) return;
	if (!(str = gst_caps_get_structure (caps, 0))) return;
	if (!gst_structure_get_int (str, "width", &width) ||
		!gst_structure_get_int (str, "height", &height)) 
		return;
	
	video_width = width;
	video_height = height;
}

/* This callback will be called when GStreamer finds some stream (audio or
 * video) in the open file. This function links the appropriate elements. */
static void cb_new_pad(GstElement* object, GstPad *pad, 
		gboolean arg1, gpointer user_data)
{
	GstCaps *caps;
	GstStructure *str;

	caps = gst_pad_get_caps (pad);
	str = gst_caps_get_structure (caps, 0);

#ifdef DEBUG
	g_print("New found stream: %s\n", gst_structure_get_name (str));
#endif

	if (g_strrstr (gst_structure_get_name (str), "video"))
	{
		if (GST_PAD_IS_LINKED (vqueuesink)) return;

		g_signal_connect(GST_PAD_REALIZE(pad),
				"notify::caps", G_CALLBACK(cb_notify_caps), NULL);
		
		if (!gst_pad_link(pad, vqueuesink))
		{
			g_error("Cannot link video\n");
			return;
		}

		gst_bin_add(GST_BIN(pipeline), vthread);
		gst_bin_sync_children_state (GST_BIN (pipeline));

		return;
	}

	if (audio_disabled) return;

	if (g_strrstr (gst_structure_get_name (str), "audio"))
	{
		if (GST_PAD_IS_LINKED (aqueuesink)) return;

		if (!gst_pad_link(pad, aqueuesink))
		{
			g_error("Cannot link audio\n");
			return;
		}

		apeer = pad;
		gst_bin_add(GST_BIN(pipeline), athread);
		gst_bin_sync_children_state (GST_BIN (pipeline));
	}
}

/* End of stream callback */
static void cb_eos(GstElement *thread, gpointer data) 
{
	video_stop();
}

/* Error callback */
static void cb_error(GstElement *thread, GstElement *source,
		GError *error, gchar *debug, gpointer data)
{
	g_print ("Error in thread %p: %s\n", g_thread_self (), error->message);
}

/* State change callback. Update the status variable. */
void cb_state_change (GstElement *gstelement, gint old_state, 
		gint new_state, gpointer user_data)
{
	g_mutex_lock(status_mutex);
	if (new_state == GST_STATE_NULL)
	{
		status &= ~VIDEO_IS_OPEN;
		status &= ~VIDEO_PLAYING;
	} 
	else 
	{
		status |= VIDEO_IS_OPEN;
		if (new_state == GST_STATE_PLAYING) 
			status |= VIDEO_PLAYING;
		else 
			status &= ~VIDEO_PLAYING;
	}
	g_mutex_unlock(status_mutex);
}

/* Get a valid sink inside the pipeline. */
GstElement *get_sink()
{
	if (GST_PAD_IS_LINKED(vqueuesink)) return xvsink;
	if (audio_disabled) return NULL;
	if (GST_PAD_IS_LINKED(aqueuesink)) return asink;
	return NULL;
}

/* Disable audio on-the-fly: */
void disable_audio()
{
	if (audio_disabled) return;
	if (!gst_bin_get_by_name(GST_BIN(pipeline), "athread")) return;
	if (!GST_PAD_IS_LINKED(aqueuesink)) return;
	if (!apeer) return;

	gst_pad_unlink(apeer, aqueuesink);
	gst_bin_remove(GST_BIN(pipeline), athread);
	gst_element_set_state(athread, GST_STATE_READY);
	gst_bin_sync_children_state (GST_BIN (pipeline));
}

/* Enable audio on-the-fly: */
void enable_audio()
{
	if (audio_disabled) return;
	if (gst_bin_get_by_name(GST_BIN(pipeline), "athread")) return;
	if (GST_PAD_IS_LINKED(aqueuesink)) return;
	if (!apeer) return;

	gst_pad_link(apeer, aqueuesink);
	gst_bin_add(GST_BIN(pipeline), athread);
	gst_bin_sync_children_state (GST_BIN (pipeline));
}

/********************** Interface functions ****************************/

/* Init the video interface. Start GStreamer and create all the elements. */
int video_init(int flags)
{
	GstElement *dec, *vqueue, *vconv, *vscale, *aqueue, *aconv, *ascale;

	if (!gst_init_check(NULL, NULL)) return VIDEO_ERROR;

	status = 0;
	status_mutex = g_mutex_new();
	video_width = video_height = 0;
	apeer = NULL;
	audio_disabled = flags & VIDEO_INIT_NOAUDIO;

	/* Main pipeline */
	pipeline = gst_thread_new("pipeline");
	g_signal_connect(pipeline, "eos", G_CALLBACK (cb_eos), NULL);
	g_signal_connect(pipeline, "error", G_CALLBACK (cb_error), NULL);
	g_signal_connect(pipeline, "state-change", G_CALLBACK (cb_state_change), NULL);
	src = gst_element_factory_make("filesrc", "src");
	dec = gst_element_factory_make("decodebin", "dec");
	g_signal_connect (dec, "new-decoded-pad", G_CALLBACK(cb_new_pad), NULL);
	if (!gst_element_link_many(src, dec, NULL)) return VIDEO_ERROR;
	gst_bin_add_many(GST_BIN(pipeline), src, dec, NULL);

	/* Video thread */
	vthread = gst_thread_new("vthread");
	vqueue = gst_element_factory_make("queue", "vqueue");
	vqueuesink = gst_element_get_pad(vqueue, "sink");
	vconv = gst_element_factory_make("ffmpegcolorspace", "vconv");
	vscale = gst_element_factory_make("videoscale", "vscale");
	vdrop = gst_element_factory_make("videodrop", "vdrop");
	xvsink = gst_element_factory_make("xvimagesink", "xvsink");
	gst_bin_add_many(GST_BIN(vthread), vqueue, vconv, vscale, vdrop, xvsink, NULL);
	if (!gst_element_link_many(vqueue, vconv, vscale, vdrop, xvsink, NULL)) 
		return VIDEO_ERROR;
	gst_object_ref(GST_OBJECT(vthread));

	/* Audio thread */
	if (audio_disabled) return VIDEO_OK;

	athread = gst_thread_new("athread");
	aqueue = gst_element_factory_make("queue", "aqueue");
	aqueuesink = gst_element_get_pad(aqueue, "sink");
	aconv = gst_element_factory_make("audioconvert", "aconv");
	ascale = gst_element_factory_make ("audioscale", "ascale");
	asink = gst_element_factory_make ("alsasink", "asink");
	gst_bin_add_many (GST_BIN (athread), aqueue, aconv, ascale, asink, NULL);
	if (!gst_element_link_many (aqueue, aconv, ascale, asink, NULL))
		return VIDEO_ERROR;
	gst_object_ref(GST_OBJECT(athread));

	return VIDEO_OK;
}

/* Specify the X11 window ID where the video will be shown. */
void video_set_xwid(unsigned long xwid)
{
	gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(xvsink), xwid);
}

/* Update the window. */
void video_expose()
{
	gst_x_overlay_expose(GST_X_OVERLAY(xvsink));
}

int video_open(const char *filename)
{
	video_close();
	g_object_set (G_OBJECT (src), "location", filename, NULL);

	if (!gst_element_set_state(pipeline, GST_STATE_READY))
		return VIDEO_ERROR;

	return VIDEO_OK;
}

int video_play(int flags)
{
	if (!gst_element_set_state(pipeline, GST_STATE_PAUSED))
		return VIDEO_ERROR;

	/* Ugly hack: Must disable audio because the GStreamer 'speed' element
	 * is broken in the 0.8.9 release. */
	if (flags & VIDEO_SLOW_2X)
	{
		disable_audio();
		g_object_set (G_OBJECT (vdrop), "speed", 0.5, NULL);
	}
	else if (flags & VIDEO_FAST_2X)
	{
		disable_audio();
		g_object_set (G_OBJECT (vdrop), "speed", 2.0, NULL);
	}
	else if (flags & VIDEO_NORMAL_SPEED)
	{
		enable_audio();
		g_object_set (G_OBJECT (vdrop), "speed", 1.0, NULL);
	}

	if (!gst_element_set_state(pipeline, GST_STATE_PLAYING))
		return VIDEO_ERROR;

	return VIDEO_OK;
}

int video_stop()
{
	if (video_pause() != VIDEO_OK) return VIDEO_ERROR;
	if (video_seek(0, VIDEO_SEEK_ABSOLUTE) != VIDEO_OK) return VIDEO_ERROR;

	return VIDEO_OK;
}

int video_pause()
{
	if (!gst_element_set_state(pipeline, GST_STATE_PAUSED))
		return VIDEO_ERROR;

	return VIDEO_OK;
}

/* 'pos' parameter is in milliseconds. */
int video_seek(int pos, int flags)
{
	gint64 len, cur;
	GstFormat fmt = GST_FORMAT_TIME;
	GstElement *sink = get_sink();

	if (!sink) return VIDEO_ERROR;
	
	if (!(gst_element_query(sink, GST_QUERY_POSITION, &fmt, &cur) &&
		  gst_element_query(sink, GST_QUERY_TOTAL, &fmt, &len))) 
		return VIDEO_ERROR;

	if (flags & VIDEO_SEEK_RELATIVE)
		cur += pos * GST_SECOND;
	else
		cur = pos * GST_SECOND;

	if (cur >= len) return video_stop();
	if (cur <= 0) cur = 0;
	
	if (!gst_element_seek(
			sink,
			GST_SEEK_METHOD_SET | GST_SEEK_FLAG_FLUSH | GST_FORMAT_TIME,
			cur))
		return VIDEO_ERROR;

	return VIDEO_OK;
}

int video_close()
{
	GstElement *thread;

	if (!gst_element_set_state(pipeline, GST_STATE_READY))
		return VIDEO_ERROR;

	/* Get the audio and video threads out of the pipeline: */
	thread = gst_bin_get_by_name(GST_BIN(pipeline), "vthread");
	if (thread) gst_bin_remove(GST_BIN(pipeline), thread);
	thread = gst_bin_get_by_name(GST_BIN(pipeline), "athread");
	if (thread) gst_bin_remove(GST_BIN(pipeline), thread);
	apeer = NULL;

	if (!gst_element_set_state (pipeline, GST_STATE_NULL))
		return VIDEO_ERROR;

	video_width = video_height = 0;
	
	return VIDEO_OK;
}

/* Free everything: */
void video_quit()
{
	video_close();
	gst_object_unref(GST_OBJECT(vthread));
	if (!audio_disabled) gst_object_unref(GST_OBJECT(athread));
	gst_object_unref(GST_OBJECT(pipeline));
}

/* Get the status variable. Must use the mutex because we are in a 
 * multithreaded environment. */
int video_get_status() 
{
	int r;

	g_mutex_lock(status_mutex);
	r = status;
	g_mutex_unlock(status_mutex);

	return r;
}

int video_get_pos_msec(unsigned long *_pos, unsigned long *_len)
{
	gint64 len, pos;
	GstFormat fmt = GST_FORMAT_TIME;
	GstElement *sink = get_sink();

	if (!sink) return VIDEO_ERROR;

	if (gst_element_query (sink, GST_QUERY_POSITION, &fmt, &pos) &&
		gst_element_query (sink, GST_QUERY_TOTAL, &fmt, &len)) 
	{
		*_pos = ((pos) / GST_MSECOND);
		*_len = ((len) / GST_MSECOND);

		return VIDEO_OK;
	}
	return VIDEO_ERROR;
}

int video_get_pos_frames(unsigned long *_pos, unsigned long *_len)
{
	gint64 len, pos;
	GstFormat fmt = GST_FORMAT_DEFAULT;
	GstElement *sink = get_sink();

	if (!sink) return VIDEO_ERROR;
	
	if (gst_element_query (sink, GST_QUERY_POSITION, &fmt, &pos) &&
		gst_element_query (sink, GST_QUERY_TOTAL, &fmt, &len)) 
	{
		*_pos = pos;
		*_len = len;
		return VIDEO_OK;
	}
	return VIDEO_ERROR;
}

/* Get the video's width & height.
 *
 * Warning: Very ugly hack. I'm not proud of it, if you're wondering.
 * 
 * This function has memory. When the video size has changed since the
 * last call (eg, when we have just open a new video), video_get_size()
 * returns 1 to reflect the change. */
int video_get_size(int *width, int *height)
{
	static int last_width = 0;
	static int last_height = 0;

	*width = video_width;
	*height = video_height;

	if ((video_width != last_width) || (video_height != last_height))
	{
		last_width = video_width;
		last_height = video_height;

		return 1;
	}

	return 0;
}

