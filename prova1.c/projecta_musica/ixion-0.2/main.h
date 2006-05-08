/* ============================================================================
 * << File: main.h >>
 * ------------------
 *  Authors: Diego Essaya <dessaya@fi.uba.ar>
 *  Date: Mon Feb 28 20:35:10 ART 2005
 *
 *  Description:
 *    Ixion's main window.
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

#ifndef _MAIN_H_
#define _MAIN_H_

#include "ixion.h"

enum
{
    ID_Quit = 0,
	ID_About,
	ID_Open,
	ID_PosTime,
	ID_PosFrames,
	ID_PlayPause,
	ID_Stop,
	ID_Rew,
	ID_Ffwd,
	ID_05X,
	ID_1X,
	ID_2X,
	ID_Timer,
};

class MainFrame;

class VideoWidget: public wxWindow
{
public:
	VideoWidget(MainFrame *_main);
	void OnPaint(wxPaintEvent &evt);
	unsigned long GetXwid();

	wxBitmap splash;
	MainFrame *main;
private:
    DECLARE_EVENT_TABLE()

	unsigned long xwid;
};

class MainFrame: public wxFrame
{
public:
	MainFrame();

	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnPlayPause(wxCommandEvent& event);
	void OnStop(wxCommandEvent& event);
	void OnFfwd(wxCommandEvent& event);
	void OnRew(wxCommandEvent& event);
	void OnSpeedBtn(wxCommandEvent& event);
	void OnTimer(wxTimerEvent &);

	void UpdateStatus();
	bool OpenFile(const wxString &filename, bool play = true);

	wxTimer timer;
	VideoWidget *video_widget;
	int video_status;
private:
    DECLARE_EVENT_TABLE()
};

#endif
