/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/AtlasEditDialog.cpp
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

#ifdef __GNUG__
#pragma implementation "AtlasEditDialog.h"
#endif

#include "AtlasEditDialog.h"

// begin wxGlade: ::extracode

// end wxGlade

#include <wx/button.h>
#include <wx/choice.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "AtlasLogic.h"
#include "AtlasAliasDialog.h"
#include "CommonInputFields.h"
#include "DialogElements.h"
#include "guivalidators.h"
#include "Lang.h"

enum { AE_COUNTRY = wxID_HIGHEST + 1, AE_ADMIN, AE_ALIASES, AE_TZNAME };

/*****************************************************
**
**   AtlasEditDialog   ---   Constructor
**
******************************************************/
AtlasEditDialog::AtlasEditDialog(wxWindow* parent, AtlasLogic *logic, AtlasEntry &entry )
 : wxDialog(parent, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ),
 logic( logic ),
 entry( entry )
{
    // begin wxGlade: AtlasEditDialog::AtlasEditDialog
    sizer_bottom_staticbox = new wxStaticBox(this, -1, wxEmptyString);
    sizer_main_staticbox = new wxStaticBox(this, -1, wxEmptyString);
    sizer_left_staticbox = new wxStaticBox(this, -1, _("Attributes"));
    label_name = new wxStaticText(this, wxID_ANY, _("Name"));
    text_name = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_asciiname = new wxStaticText(this, wxID_ANY, _("ASCII Name"));
    text_asciiname = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_country = new wxStaticText(this, wxID_ANY, _("Country"));
    const wxString *choice_country_choices = NULL;
    choice_country = new wxChoice(this, AE_COUNTRY, wxDefaultPosition, wxDefaultSize, 0, choice_country_choices, 0);
    label_admincode = new wxStaticText(this, wxID_ANY, _("Admin Code"));
    const wxString *choice_admin_choices = NULL;
    choice_admin = new wxChoice(this, AE_ADMIN, wxDefaultPosition, wxDefaultSize, 0, choice_admin_choices, 0);
    label_population = new wxStaticText(this, wxID_ANY, _("Population"));
    text_population = new IntegerInputField(this, wxID_ANY);
    label_longitude = new wxStaticText(this, wxID_ANY, _("Longitude"));
    text_longitude = new LongitudeInputField(this, wxID_ANY);
    const wxString choice_longitude_choices[] = {
        _("East"),
        _("West")
    };
    choice_longitude = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_longitude_choices, 0);
    label_latitude = new wxStaticText(this, wxID_ANY, _("Latitude"));
    text_latitude = new LatitudeInputField(this, wxID_ANY);
    const wxString choice_latitude_choices[] = {
        _("North"),
        _("South")
    };
    choice_latitude = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_latitude_choices, 0);
    label_timezone = new wxStaticText(this, wxID_ANY, _("Time Zone"));
    const wxString *choice_timezone_choices = NULL;
    choice_timezone = new wxChoice(this, AE_TZNAME, wxDefaultPosition, wxDefaultSize, 0, choice_timezone_choices, 0);
    label_tzh = new wxStaticText(this, wxID_ANY, _("TZ Hours"));
    label_tzhours = new wxStaticText(this, wxID_ANY, _("dummy"));
    label_aliases = new wxStaticText(this, wxID_ANY, _("Aliases"));
    button_aliases = new wxButton(this, AE_ALIASES, _("Edit Aliases ..."));
    text_alias = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY);
    button_ok = new wxButton(this, wxID_OK, _("OK"));
    button_cancel_copy = new wxButton(this, wxID_CANCEL, _("Cancel"));

    set_properties();
    do_layout();
    // end wxGlade

	setData();
 
	Connect( AE_COUNTRY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AtlasEditDialog::OnChoiceCountry ));
	Connect( AE_TZNAME, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AtlasEditDialog::OnChoiceTzName ));
	Connect( AE_ALIASES, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasEditDialog::OnEntryAliases ));
	Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AtlasEditDialog::OnOK ));
	Connect( wxID_ANY, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AtlasEditDialog::OnTextChanged ));

	text_name->SetFocus();
}

/*****************************************************
**
**   AtlasEditDialog   ---   saveData
**
******************************************************/
void AtlasEditDialog::saveData()
{
	entry.name = text_name->GetValue();
	entry.asciiname = text_asciiname->GetValue();

	entry.country = choice_country->GetStringSelection();
	entry.country_code = logic->getCountryCodeForName( choice_country->GetStringSelection());

	entry.admin = choice_admin->GetStringSelection();
	entry.admin1_code = logic->getAdminCodeForCountryAndName( entry.country_code, choice_admin->GetStringSelection());

	entry.population = (int)text_population->getDoubleValue();

	entry.longitude = text_longitude->getDoubleValue();
	if ( choice_longitude->GetSelection() ) entry.longitude *= -1;

	entry.latitude = text_latitude->getDoubleValue();
	if ( choice_latitude->GetSelection() ) entry.latitude *= -1;

	entry.aliases = text_alias->GetValue();
	if ( entry.aliases.IsEmpty() ) entry.aliases = entry.name;
}

/*****************************************************
**
**   AtlasEditDialog   ---   setData
**
******************************************************/
void AtlasEditDialog::setData()
{
	text_name->SetValue( entry.name );
	text_asciiname->SetValue( entry.asciiname );
	if ( entry.population > 0 ) text_population->SetValue( wxString::Format( wxT( "%d" ), entry.population ));

  text_latitude->setDoubleValue( fabs( entry.latitude ));
	choice_latitude->SetSelection( entry.latitude < 0 );

  text_longitude->setDoubleValue( fabs( entry.longitude ) );
	choice_longitude->SetSelection( entry.longitude < 0 );

	choice_country->SetStringSelection( entry.country );
	updateAdminChoice();
	updateTzChoice();

	text_alias->SetValue( entry.aliases );
}

/*****************************************************
**
**   AtlasEditDialog   ---   updateAdminChoice
**
******************************************************/
void AtlasEditDialog::updateAdminChoice()
{
	choice_admin->Clear();
	choice_admin->Append( wxEmptyString );

	const list<wxString> &l = logic->getAllAdminNamesForCountry( choice_country->GetStringSelection());
	for ( list<wxString>::const_iterator iter = l.begin(); iter != l.end(); iter++ )
	{
		choice_admin->Append( *iter );
	}
	if ( choice_admin->FindString( entry.admin ) != wxNOT_FOUND ) choice_admin->SetStringSelection( entry.admin );
	else choice_admin->SetSelection( 1 );
}

/*****************************************************
**
**   AtlasEditDialog   ---   OnChoiceCountry
**
******************************************************/
void AtlasEditDialog::OnChoiceCountry( wxCommandEvent &event )
{
	updateAdminChoice();
	updateTzChoice();
}

/*****************************************************
**
**   AtlasEditDialog   ---   OnChoiceTzName
**
******************************************************/
void AtlasEditDialog::OnChoiceTzName( wxCommandEvent &event )
{
	updateTzHours();
}

/*****************************************************
**
**   AtlasEditDialog   ---   updateTzHours
**
******************************************************/
void AtlasEditDialog::updateTzHours()
{
	wxString tzfullname = choice_timezone->GetStringSelection();

	for ( list<TimezoneEntry>::iterator iter = tzlist.begin(); iter != tzlist.end(); iter++ )
	{
		if ( (*iter).name == tzfullname )
		{
			label_tzhours->SetLabel( wxString::Format( wxT( "%02.1f" ), (*iter).tzoffset));
			return;
		}
	}
	printf( "ERROR: no time zone found for entry %s\n", str2char( tzfullname ));
}

/*****************************************************
**
**   AtlasEditDialog   ---   updateTzChoice
**
******************************************************/
void AtlasEditDialog::updateTzChoice()
{
	wxString country_code;
	wxString country = choice_country->GetStringSelection();

	// get country
	for ( list<AtlasCountry>::iterator iter = countrylist.begin(); iter != countrylist.end(); iter++ )
	{
		if ( (*iter).name == country )
		{
			country_code = (*iter).iso;
			break;
		}
	}

	choice_timezone->Clear();
	for ( list<TimezoneEntry>::iterator iter = tzlist.begin(); iter != tzlist.end(); iter++ )
	{
		if ( (*iter).country_code != country_code ) continue;

		wxString tzname = (*iter).name;
		choice_timezone->Append( tzname );
	}
	if ( choice_timezone->FindString( entry.timezone ) != wxNOT_FOUND ) choice_timezone->SetStringSelection( entry.timezone );
	else choice_timezone->SetSelection( 0 );

	updateTzHours();
}

/*****************************************************
**
**   AtlasEditDialog   ---   OnEntryAliases
**
******************************************************/
void AtlasEditDialog::OnEntryAliases( wxCommandEvent &event )
{
	AtlasAliasDialog dialog( this, entry );
	if ( dialog.ShowModal() == wxID_OK )
	{
		text_alias->SetValue( entry.aliases );
	}
}

/*****************************************************
**
**   AtlasEditDialog   ---   OnOk
**
******************************************************/
void AtlasEditDialog::OnOK( wxCommandEvent& )
{
	saveData();
	EndModal( wxID_OK );
}

/*****************************************************
**
**   AtlasEditDialog   ---   OnTextChanged
**
******************************************************/
void AtlasEditDialog::OnTextChanged( wxCommandEvent &event )
{
	Validate();
	event.Skip();
}

/*****************************************************
**
**   AtlasEditDialog   ---   set_properties
**
******************************************************/
void AtlasEditDialog::set_properties()
{
	countrylist = logic->getAllCountries();
	for ( list<AtlasCountry>::iterator iter = countrylist.begin(); iter != countrylist.end(); iter++ )
	{
		choice_country->Append( (*iter).name );
	}
	tzlist = logic->getAllTimezones();

    // begin wxGlade: AtlasEditDialog::set_properties
    SetTitle(_("Edit Atlas Entry"));
    choice_longitude->SetMinSize(wxSize(100, -1));
    choice_longitude->SetSelection(0);
    choice_latitude->SetMinSize(wxSize(100, -1));
    choice_latitude->SetSelection(0);
    text_alias->SetMinSize(wxSize(-1, 100));
    // end wxGlade
}

/*****************************************************
**
**   AtlasEditDialog   ---   do_layout
**
******************************************************/
void AtlasEditDialog::do_layout()
{
    // begin wxGlade: AtlasEditDialog::do_layout
    wxStaticBoxSizer* sizer_main = new wxStaticBoxSizer(sizer_main_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_bottom = new wxStaticBoxSizer(sizer_bottom_staticbox, wxHORIZONTAL);
    wxStaticBoxSizer* sizer_left = new wxStaticBoxSizer(sizer_left_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_attributes = new wxFlexGridSizer(11, 2, 0, 0);
    wxBoxSizer* sizer_latitude = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_longitude = new wxBoxSizer(wxHORIZONTAL);
    grid_attributes->Add(label_name, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_attributes->Add(text_name, 0, wxALL|wxEXPAND, 3);
    grid_attributes->Add(label_asciiname, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_attributes->Add(text_asciiname, 0, wxALL|wxEXPAND, 3);
    grid_attributes->Add(label_country, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_attributes->Add(choice_country, 0, wxALL, 3);
    grid_attributes->Add(label_admincode, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_attributes->Add(choice_admin, 0, wxALL, 3);
    grid_attributes->Add(label_population, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_attributes->Add(text_population, 1, wxALL|wxEXPAND, 3);
    grid_attributes->Add(label_longitude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_longitude->Add(text_longitude, 1, wxALL|wxEXPAND, 3);
    sizer_longitude->Add(choice_longitude, 0, wxALL, 3);
    grid_attributes->Add(sizer_longitude, 1, wxALL|wxEXPAND, 0);
    grid_attributes->Add(label_latitude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_latitude->Add(text_latitude, 1, wxALL|wxEXPAND, 3);
    sizer_latitude->Add(choice_latitude, 0, wxALL, 3);
    grid_attributes->Add(sizer_latitude, 1, wxALL|wxEXPAND, 0);
    grid_attributes->Add(label_timezone, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_attributes->Add(choice_timezone, 1, wxALL, 3);
    grid_attributes->Add(label_tzh, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_attributes->Add(label_tzhours, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_attributes->Add(label_aliases, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_attributes->Add(button_aliases, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_attributes->AddGrowableCol(1);
    sizer_left->Add(grid_attributes, 0, wxEXPAND, 0);
    sizer_left->Add(text_alias, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_left, 1, wxEXPAND, 0);
    sizer_bottom->Add(button_ok, 0, wxALL, 3);
    sizer_bottom->Add(button_cancel_copy, 0, wxALL, 3);
    sizer_main->Add(sizer_bottom, 0, wxALIGN_CENTER_HORIZONTAL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    Layout();
    // end wxGlade
}

