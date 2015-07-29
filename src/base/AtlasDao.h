/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/AtlasDao.h
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

#ifndef ATLASDAO_H
#define ATLASDAO_H

#ifdef __GNUG__
#pragma interface "AtlasDao.h"
#endif

#include "BaseDao.h"

/*************************************************//**
*
*
*
******************************************************/
struct AtlasEntry {

	AtlasEntry()
	{
		clear();
	}

	void clear()
	{
		id = rowid = 0;
		name.Clear();
		asciiname.Clear();
		aliases.Clear();
		country_code.Clear();
		country.Clear();
		feature_code.Clear();
		feature_class.Clear();
		feature_name.Clear();
		admin1_code.Clear();
		admin.Clear();
		population = elevation = 0;
		longitude = latitude = 0;
		timezone.Clear();
		tzoffset = 0;
	}

	int id;
	int rowid;
	wxString name;
	wxString asciiname;
	wxString aliases;
	wxString country_code;
	wxString country;
	wxString feature_code;
	wxString feature_class;
	wxString feature_name;
	wxString admin1_code;
	wxString admin;
	int population;
	int elevation;
	double longitude;
	double latitude;
	wxString timezone;
	double tzoffset;
};

/*************************************************//**
*
*
*
******************************************************/
struct AtlasCountry
{
	AtlasCountry( wxString iso, wxString name )
	{
		this->iso = iso;
		this->name = name;
	}
	wxString iso;
	wxString name;
};

/*****************************************************
**
**   class AtlasCountrySorter
**
******************************************************/
class AtlasCountrySorter
{
public:
	bool operator()( const AtlasCountry &c1, const AtlasCountry &c2 ) const
	{
		return( c1.name.CmpNoCase( c2.name ) < 0 );
	}
};

/*************************************************//**
*
*
*
******************************************************/
struct TimezoneEntry
{

	TimezoneEntry( wxString name, wxString country_code, const double &offset )
	 : name( name ),
	   country_code( country_code ),
	   tzoffset( offset )
	{
	}

	wxString name;
	wxString country_code;
	double tzoffset;
};

/*************************************************//**
*
*
*
******************************************************/
class AtlasDao : public BaseDao
{
public:

	AtlasDao( const wxString = wxEmptyString );

	vector<AtlasEntry> getEntries( wxString filter, wxString country, const int &mode, const int &limit, const int &offset );

	AtlasEntry getFullEntry( int id );

	int getMatchCount( wxString filter, wxString country, const int &mode );

	list<AtlasCountry> getAllCountries();

	wxString getCountryName( wxString iso );

	wxString getFeatureName( wxString feature_class, wxString feature_code );

	wxString getAdminName( wxString country_code, wxString admin1_code );

	wxString getCountryCodeForName( wxString name );

	wxString getAdminCodeForCountryAndName( wxString country_code, wxString name );

	list<wxString> getAllAdminNamesForCountry( const wxString &country_code );

	list<TimezoneEntry> getAllTimezones();

  void saveEntry( AtlasEntry& );

	void saveAliasNames( const int &id, wxString country_code, wxString names );

	void saveExistingEntry( const AtlasEntry &entry );

	void saveNewEntry(  AtlasEntry &entry );

	void deleteEntry( const int& id );

private:

	bool entryExists( const int &id );

	int getNewFeatureId();

};


#endif

