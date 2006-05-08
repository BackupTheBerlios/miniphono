/* ============================================================================
 * << File: about.c >>
 * -------------------
 *  Authors: Diego Essaya <dessaya@fi.uba.ar>
 *  Date: Mon Feb 28 20:35:10 ART 2005
 *
 *  Description:
 *    "About..." dialog.
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

#include "about.h"

BEGIN_EVENT_TABLE(AboutDlg, wxDialog)
END_EVENT_TABLE()

AboutDlg::AboutDlg(wxWindow *main)
	: wxDialog(main, -1, _("About Ixion"),
			wxDefaultPosition, wxSize(400,400), 
			wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(
			new wxTextCtrl(this, -1, 
_(
"Ixion media player - Version 0.2\n\n"

"Copyright (C) 2005 Diego Essaya\n\n"

"Ixion is free software; you can redistribute it and/or modify "
"it under the terms of the GNU General Public License as published by "
"the Free Software Foundation; either version 2 of the License, or "
"(at your option) any later version.\n\n"

"Ixion is distributed in the hope that it will be useful, "
"but WITHOUT ANY WARRANTY; without even the implied warranty of "
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
"GNU Library General Public License for more details.\n\n"

"You should have received a copy of the GNU General Public License "
"along with this program; if not, write to the Free Software "
"Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.\n"
),
				wxDefaultPosition, wxDefaultSize, 
				wxTE_MULTILINE | wxTE_READONLY | wxTE_WORDWRAP),
			1, wxEXPAND | wxALL, 5);
	sizer->Add(new wxButton(this, wxID_OK, _("&Close")), 0, 
			wxALIGN_CENTER | wxBOTTOM, 5);
	SetSizer(sizer);

	Center();
}

