/* ============================================================================
 * << File: video.h >>
 * -------------------
 *  Authors: Diego Essaya <dessaya@fi.uba.ar>
 *  Date: Mon Feb 28 20:35:10 ART 2005
 *
 *  Description:
 *    Generic interface for video playing routines.
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

#ifndef _VIDEO_H_
#define _VIDEO_H_

/* Flags for video_init() */
#define VIDEO_INIT_NOAUDIO	1

/* Status flags: */
#define	VIDEO_IS_OPEN		1
#define	VIDEO_PLAYING		2

/* Flags for video_play() */
#define VIDEO_FAST_2X		1
#define VIDEO_NORMAL_SPEED	2
#define	VIDEO_SLOW_2X		4

/* Flags for video_seek() */
#define VIDEO_SEEK_ABSOLUTE	0
#define VIDEO_SEEK_RELATIVE	1

enum {
	VIDEO_OK = 0,
	VIDEO_ERROR = -1,
};

int video_init(int flags);
int video_open(const char *nombre);
int video_play(int flags);
int video_stop();
int video_pause();
int video_seek(int pos, int flags);
int video_close();
void video_quit();
int video_get_status();
int video_get_pos_msec(unsigned long *pos, unsigned long *len);
int video_get_pos_frames(unsigned long *pos, unsigned long *len);
int video_get_size(int *width, int *height);
void video_set_xwid(unsigned long xwid);
void video_expose();

#endif

