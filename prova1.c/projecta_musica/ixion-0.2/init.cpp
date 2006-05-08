/* ============================================================================
 * << File: init.cpp >>
 * --------------------
 *  Authors: Diego Essaya <dessaya@fi.uba.ar>
 *  Date: Mon Feb 28 20:35:10 ART 2005
 *
 *  Description:
 *    Application initialization.
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

#include "ixion.h"
#include "main.h"
extern "C"
{
#include "video.h"
}

class TpApp: public wxApp
{
    virtual bool OnInit();
};

IMPLEMENT_APP(TpApp)

bool TpApp::OnInit()
{
	int init_flags = 0;
	wxString filename;

	for (int i = 1; i < argc; i++)
	{
		wxString arg(argv[i]);
		if (arg.IsSameAs(_("-noaudio")))
		{
			init_flags = VIDEO_INIT_NOAUDIO;
			wxSafeShowMessage(_("Warning"), 
				_("Disabling audio."));
		}
		else
		{
			filename = arg;
		}
	}

	if (video_init(init_flags) != VIDEO_OK)
	{
		wxMessageBox(_("Error while starting GStreamer"), _("Error"),
				wxOK | wxICON_ERROR);
		return FALSE;
	}

	MainFrame *frame = new MainFrame();
	frame->Show();
	if (!filename.IsEmpty()) frame->OpenFile(filename);

	return TRUE;
}

