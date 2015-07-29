/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/AtlasPanel.h
 *  Release    7.0
 *  Author     Martin Pettau
 *  Copyright  2003-2012 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
****************************************************************************/

#ifndef ATLASPANEL_H
#define ATLASPANEL_H

#ifdef __GNUG__
#pragma interface "AtlasPanel.h"
#endif

#include "ConfigPanel.h"


// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode

// end wxGlade

class wxButton;
class wxCheckBox;
class wxListBox;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* \brief ConfigPanel for atlas configuration
*
******************************************************/
class AtlasPanel: public ConfigPanel
{
	DECLARE_CLASS( AtlasPanel )

    // begin wxGlade: AtlasPanel::ids
    // end wxGlade

	AtlasPanel( wxWindow* parent );
	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: AtlasPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnChooseDbFile( wxCommandEvent& );
	void OnDbFileTextChanged( wxCommandEvent& );
	bool updateDbFileStatus();

	void OnChooseSqlFile( wxCommandEvent& );
	void OnSqlFileTextChanged( wxCommandEvent& );
	void updateSqlFileStatus();

	void OnIdle( wxIdleEvent& );
	void OnStartImport( wxCommandEvent& );

	void OnCountries( wxCommandEvent& );
	void updateCountryList();

	bool getFileStatus( wxString filename, wxString &message );

	wxString countries;
	bool dirty;

protected:
    // begin wxGlade: AtlasPanel::attributes
    wxStaticBox* sizer_favcountries_staticbox;
    wxStaticBox* sizer_import_staticbox;
    wxStaticBox* sizer_db_staticbox;
    wxButton* button_dbfile;
    wxTextCtrl* text_dbfile;
    wxStaticText* text_dbstatus;
    wxPanel* panel_status;
    wxButton* button_sqlfile;
    wxTextCtrl* text_sqlfile;
    wxStaticText* text_sqlfilestatus;
    wxPanel* panel_sqlfilestatus;
    wxButton* button_startimport;
    wxListBox* list_countries;
    wxButton* button_countries;
    // end wxGlade
}; // wxGlade: end class


#endif // ATLASPANEL_H
