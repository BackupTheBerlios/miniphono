/* ============================================================================
 * << File: main.cpp >>
 * --------------------
 *  Authors: Diego Essaya <dessaya@fi.uba.ar>
 *  Date: Mon Feb 28 20:35:10 ART 2005
 *
 *  Description:
 *    Ixion main window.
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

#include "main.h"
#include "about.h"
#include <wx/gtk/win_gtk.h>
#include <wx/filename.h>
extern "C"
{
#include "video.h"
}

// Toolbar icons:
#include "bitmaps/eject.xpm"
#include "bitmaps/play.xpm"
#include "bitmaps/pause.xpm"
#include "bitmaps/stop.xpm"
#include "bitmaps/slow.xpm"
#include "bitmaps/fast.xpm"
#include "bitmaps/lento.xpm"
#include "bitmaps/normal.xpm"
#include "bitmaps/rapido.xpm"

// Splash image (cool, huh?):
#define SPLASH_PNG "ixion-splash.png"

// Ugly hack #1:
// This macro obtains the GdkWindow* of a wxWindow*. Had to do this because
// window->GetHandle() does not work with GTK+2.
#define GET_GDK_WINDOW(wxwindow) GTK_PIZZA(wxwindow->m_wxwindow)->bin_window


//
// VideoWidget -- is the widget where the video plays. You guessed it.
//

BEGIN_EVENT_TABLE(VideoWidget, wxWindow)
	EVT_PAINT(VideoWidget::OnPaint)
END_EVENT_TABLE()

VideoWidget::VideoWidget(MainFrame *_main):
	wxWindow((wxWindow *)_main, -1, wxDefaultPosition, wxDefaultSize)
{
	wxImage img;
	
	main = _main;
	xwid = 0;

	SetBackgroundColour(wxColour(_("black")));
	wxImage::AddHandler(new wxPNGHandler);
	if (!splash.LoadFile(_("./" SPLASH_PNG)))
		if (!splash.LoadFile(_(PIXMAPSDIR "/" SPLASH_PNG)))
		{
			wxSafeShowMessage(_("warning"), 
				wxString::Format(_("Could not load splash image: %s"), 
					_(PIXMAPSDIR "/" SPLASH_PNG)));
			return;
		}
	int w = splash.GetWidth();
	int h = splash.GetHeight();
	SetSize(w, h);
}

// Show splash image, if no video is open:
void VideoWidget::OnPaint(wxPaintEvent &evt)
{
	wxPaintDC dc(this);

	if (!(main->video_status & VIDEO_IS_OPEN))
	{
		if (splash.Ok() && !(main->video_status & VIDEO_PLAYING))
		{
			int w = splash.GetWidth();
			int h = splash.GetHeight();
			wxMemoryDC mdc;
			mdc.SelectObject(splash);
			dc.Blit(0, 0, w, h, &mdc, 0, 0);
		}
	}
}

// Get this window's X window ID.
unsigned long VideoWidget::GetXwid()
{
	if (!xwid)
	{
		xwid = GDK_WINDOW_XWINDOW(GET_GDK_WINDOW(this));
	}

	return xwid;
}


//
// MainFrame -- Ixion's main window. You guessed again.
//

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_CLOSE(MainFrame::OnClose)
	EVT_MENU(ID_Quit,   MainFrame::OnQuit)
	EVT_MENU(ID_About,  MainFrame::OnAbout)
	EVT_MENU(ID_Open,   MainFrame::OnOpen)
	EVT_MENU(ID_PlayPause,   MainFrame::OnPlayPause)
	EVT_MENU(ID_Stop,   MainFrame::OnStop)
	EVT_MENU(ID_Ffwd,   MainFrame::OnFfwd)
	EVT_MENU(ID_Rew,    MainFrame::OnRew)
	EVT_TOOL_RANGE(ID_05X, ID_2X, MainFrame::OnSpeedBtn)
	EVT_TIMER(ID_Timer, MainFrame::OnTimer)
END_EVENT_TABLE()

MainFrame::MainFrame() :
	wxFrame((wxFrame *)NULL, -1, 
		_("Ixion media player"), wxDefaultPosition,
	wxSize(450,500), wxDEFAULT_FRAME_STYLE | wxMAXIMIZE), 
	timer(this, ID_Timer)
{
	// Create the menu bar:
	wxMenuBar *menuBar = new wxMenuBar;

	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_Open, 
		_("&Open...\tCtrl-O"), _("Open a video."));
	menuFile->AppendSeparator();
	menuFile->Append(ID_Quit, 
		_("&Quit\tAlt-X"), _("Quit."));
	menuBar->Append(menuFile,  _("&File"));

	wxMenu *menuView = new wxMenu;
	menuView->Append(ID_PosTime,
		_("Position in &time"), 
		_("Show position in time units."),
		wxITEM_RADIO);
	menuView->Append(ID_PosFrames, 
		_("Position in &frames"),
		_("Show position in frames."),
		wxITEM_RADIO);
	menuBar->Append(menuView, _("&View"));

	wxMenu *menuAyuda = new wxMenu;
	menuAyuda->Append(ID_About, _("&About..."), 
		_("Information about Ixion."));
	menuBar->Append(menuAyuda, _("&Help"));

	SetMenuBar( menuBar );

	// Creation of the status bar:
	CreateStatusBar(2, 0);
	int widths[] = {-1, 200};
	SetStatusWidths(2, widths);
	SetStatusText(_("Welcome to Ixion."));

	// Creación de la toolbar de botones:
	wxToolBar *toolbar = CreateToolBar();
	toolbar->AddTool(ID_Open, _("^"), wxBitmap(eject_xpm), wxNullBitmap,
		wxITEM_NORMAL, _("Open"), _("Open a video."));
	toolbar->AddSeparator();
	toolbar->AddTool(ID_PlayPause, _(">"), wxBitmap(play_xpm), wxNullBitmap,
		wxITEM_NORMAL, _("Play"), _("Start playing."));
	toolbar->EnableTool(ID_PlayPause, false);
	toolbar->AddTool(ID_Stop, _("[]"), wxBitmap(stop_xpm), wxNullBitmap,
		wxITEM_NORMAL, _("Stop"), _("Stop playing."));
	toolbar->EnableTool(ID_Stop, false);
	toolbar->AddTool(ID_Rew, _("<<"), wxBitmap(slow_xpm), wxNullBitmap,
		wxITEM_NORMAL, _("Rewind"), _("Go back some seconds."));
	toolbar->EnableTool(ID_Rew, false);
	toolbar->AddTool(ID_Ffwd, _(">>"), wxBitmap(fast_xpm), wxNullBitmap,
		wxITEM_NORMAL, _("Fast forward"), _("Go forth some seconds."));
	toolbar->EnableTool(ID_Ffwd, false);
	toolbar->AddSeparator();
	toolbar->AddRadioTool(ID_05X, _("0.5X"), wxBitmap(lento_xpm), wxNullBitmap,
		_("Half speed"), _("Play at half the normal speed."));
	toolbar->AddRadioTool(ID_1X, _("1X"), wxBitmap(normal_xpm), wxNullBitmap,
		_("Normal speed"), _("Play at normal speed."));
	toolbar->AddRadioTool(ID_2X, _("2X"), wxBitmap(rapido_xpm), wxNullBitmap,
		_("Double speed"), _("Play at double the normal speed."));
	toolbar->ToggleTool(ID_1X, true);
	toolbar->AddSeparator();
	toolbar->Realize();

	// Creation of the video widget inside the main window:
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	video_widget = new VideoWidget(this);
	sizer->Add(video_widget, 1, wxEXPAND);
	SetSizer(sizer);

	// Adjust the window size to the splash image's size.
	if (video_widget->splash.Ok()) sizer->Fit(this);

	CenterOnScreen();

	// Specify the window's minimum size:
	SetSizeHints(200, 200);

	// Initialize the video_status variable:
	video_status = video_get_status();

	// Update window every 100 ms:
	timer.Start(100);
}

void MainFrame::OnClose(wxCloseEvent& event)
{
	video_quit();
	event.Skip();
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close();
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	AboutDlg dlg(this);
	dlg.ShowModal();
}

void MainFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
	wxString filename = wxFileSelector(_("Open video"), _(""), _(""), _(""), 
		_("Video files|*.mpg;*.mpeg;*.avi;*.mov|"
		"Audio files|*.ogg;*.mp3;*.wav|"
		"MPEG files|*.mpg;*.mpeg|"
		"AVI files|*.avi|"
		"Quick Time files|*.mov|"
		"OGG files|*.ogg|"
		"MP3 files|*.mp3|"
		"WAV files|*.wav|"
		"All files (*.*)|*"),
		wxOPEN | wxCHANGE_DIR | wxFILE_MUST_EXIST);

	if (!filename.IsEmpty())
	{
		OpenFile(filename);
	}
}

bool MainFrame::OpenFile(const wxString &filename, bool autoplay)
{
	// Make sure the X window ID is set:
	video_set_xwid(video_widget->GetXwid());

	// Open the file:
	if (video_open(filename.mb_str()) != VIDEO_OK)
	{
		wxMessageBox(_("Cannot open file."), _("Error"),
				wxOK | wxICON_ERROR, this);
		video_close();
		return false;
	}

	// Start playing:
	if (autoplay)
	{
		UpdateStatus();
		ProcessCommand(ID_PlayPause);
	}

	return true;
}

void MainFrame::OnPlayPause(wxCommandEvent& WXUNUSED(event))
{
	int r;

	if (video_status & VIDEO_PLAYING)
	{
		r = video_pause();
	}
	else
	{
		wxToolBar *toolbar = GetToolBar();
		if (toolbar->GetToolState(ID_05X)) r = video_play(VIDEO_SLOW_2X);
		else if (toolbar->GetToolState(ID_2X)) r = video_play(VIDEO_FAST_2X);
		else r = video_play(VIDEO_NORMAL_SPEED);
	}

	if (r != VIDEO_OK)
	{
		wxMessageBox(_("Cannot play/pause video."), _("Error"),
					wxOK | wxICON_ERROR, this);
	}
}

void MainFrame::OnStop(wxCommandEvent& WXUNUSED(event))
{
	if (video_stop() != VIDEO_OK)
	{
		wxMessageBox(_("Cannot stop video."), _("Error"),
					wxOK | wxICON_ERROR, this);
	}
}

void MainFrame::OnFfwd(wxCommandEvent& WXUNUSED(event))
{
	if (video_seek(15, VIDEO_SEEK_RELATIVE) != VIDEO_OK)
	{
		wxMessageBox(_("Cannot seek video."), _("Error"),
					wxOK | wxICON_ERROR, this);
	}
}

void MainFrame::OnRew(wxCommandEvent& WXUNUSED(event))
{
	if (video_seek(-15, VIDEO_SEEK_RELATIVE) != VIDEO_OK)
	{
		wxMessageBox(_("Cannot seek video."), _("Error"),
					wxOK | wxICON_ERROR, this);
	}
}

void MainFrame::OnSpeedBtn(wxCommandEvent& event)
{
	if (video_status & VIDEO_PLAYING)
	{
		int r;

		wxToolBar *toolbar = GetToolBar();
		if (toolbar->GetToolState(ID_05X)) r = video_play(VIDEO_SLOW_2X);
		else if (toolbar->GetToolState(ID_2X)) r = video_play(VIDEO_FAST_2X);
		else r = video_play(VIDEO_NORMAL_SPEED);

		if (r != VIDEO_OK)
		{
			wxMessageBox(_("Cannot play video"), _("Error"),
						wxOK | wxICON_ERROR, this);
		}
	}
}

// This function is called every 100 ms:
void MainFrame::OnTimer(wxTimerEvent& evt)
{
	UpdateStatus();
}

// Update window:
void MainFrame::UpdateStatus()
{
	int new_status = video_get_status();
	wxToolBar *toolbar = GetToolBar();

	// changed_status holds the flags that have changed since the last time:
	int changed_status = video_status ^ new_status;
	video_status = new_status;
	
	if (changed_status & VIDEO_IS_OPEN)
	{
		bool open = video_status & VIDEO_IS_OPEN;
		
		toolbar->EnableTool(ID_PlayPause, open);
		toolbar->EnableTool(ID_Stop, open);
		toolbar->EnableTool(ID_Ffwd, open);
		toolbar->EnableTool(ID_Rew, open);

		// Ugly hack #3: See ugly hack #2.
		if (!open)
			gdk_window_thaw_updates(GET_GDK_WINDOW(video_widget));
	}
	
	if (changed_status & VIDEO_PLAYING)
	{
		toolbar->RemoveTool(ID_PlayPause);

		if (video_status & VIDEO_PLAYING)
		{
			toolbar->InsertTool(2, ID_PlayPause, _("||"), wxBitmap(pause_xpm),
				wxNullBitmap, wxITEM_NORMAL, 
				_("Pause"), _("Pause the video."));
		}
		else
		{
			toolbar->InsertTool(2, ID_PlayPause, _(">"), wxBitmap(play_xpm),
				wxNullBitmap, wxITEM_NORMAL, 
				_("Play"), _("Start playing."));
		}

		toolbar->Realize();
	}

	if (!(video_status & VIDEO_IS_OPEN))
	{
		SetStatusText(_(""), 1);
		return;
	}

	int width, height;

	if (video_get_size(&width, &height))
	{
		// We have just obtained a video with a new different size. => Adjust
		// the window size.
		if (width && height)
		{
			wxSizer *s = GetSizer();
			video_widget->SetSize(width, height);
			s->Fit(this);
			CenterOnScreen();
		}
	}

	// Show stream position in the status bar:
	unsigned long pos, len;
	wxMenuBar *menu = GetMenuBar();

	if (menu->IsChecked(ID_PosFrames))
	{
		if (video_get_pos_frames(&pos, &len) != VIDEO_OK) 
		{
			SetStatusText(_(""), 1);
		}
		else
		{
			SetStatusText(wxString::Format(_("%d / %d"), pos, len), 1);
		}
	}
	else
	{
		if (video_get_pos_msec(&pos, &len) != VIDEO_OK) 
		{
			SetStatusText(_(""), 1);
		}
		else
		{
			wxTimeSpan tsp(0, 0, 0, pos), tsl(0, 0, 0, len);
			SetStatusText(tsp.Format(_("%H:%M:%S.%l")) + _(" / ") +
					tsl.Format(_("%H:%M:%S.%l")), 1);
		}
	}

	// Ugly hack #2:
	// We must avoid the repainting of the video widget; but it seems that
	// wxWindow::Freeze() is broken in GTK+2.
	gdk_window_freeze_updates(GET_GDK_WINDOW(video_widget));
	video_expose();
}

