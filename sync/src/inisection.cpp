/***************************************************************************
 *   Copyright (C) 2005 by Matthew Martin   *
 *   matthew@nabiezzi.plus.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "inisection.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

#include "log.h"

iniSection::iniSection(const std::string& sSection)
: m_sSectionName(sSection)
{

}


iniSection::~iniSection()=default;

/*!
    \fn iniSection::GetDataBegin()
 */
mapIniData::const_iterator iniSection::GetDataBegin() const
{
    return m_mSectionData.begin();
}

/*!
    \fn iniSection::GetDataEnd()
 */
mapIniData::const_iterator iniSection::GetDataEnd() const
{
    return m_mSectionData.end();
}

const mapIniData& iniSection::GetData() const
{
    return m_mSectionData;
}


/*!
    \fn iniSection::GetString(const std::string& sKey, const std::string& sDefault)
 */
const std::string& iniSection::Get(const std::string& sKey, const std::string& sDefault) const
{
  	//does the key exist
  	auto it = m_mSectionData.find(sKey);
	if(it==m_mSectionData.end())
	{	
        return sDefault;
    }
	return it->second;
}

/*!
    \fn iniSection::GetInt(const std::string& sKey, int nDefault)
 */
long iniSection::Get(const std::string& sKey, long nDefault) const
{
  	//does the key exist
  	auto it = m_mSectionData.find(sKey);
	if(it==m_mSectionData.end())
    {
		return nDefault;
    }
    try
    {
        return std::stol(it->second);
    }
    catch(const std::invalid_argument& e)
    {
        return nDefault;
    }
    catch(const std::out_of_range& e)
    {
        return nDefault;
    }
}

/*!
    \fn iniSection::Getstd::string(const std::string& sKey, double dDefault)
 */
double iniSection::Get(const std::string& sKey, double dDefault) const
{
  	//does the key exist
	auto it = m_mSectionData.find(sKey);
	if(it==m_mSectionData.end())
	{
        return dDefault;
    }
    try
    {
        return std::stod(it->second);
    }
    catch(const std::invalid_argument& e)
    {
        return dDefault;
    }
    catch(const std::out_of_range& e)
    {
        return dDefault;
    }
}

bool iniSection::Get(const std::string& sKey, bool bDefault) const
{
  	//does the key exist
	auto it = m_mSectionData.find(sKey);
	if(it==m_mSectionData.end())
	{
        return bDefault;
    }

    return (it->second == "1" || it->second == "true" || it->second == "TRUE" || it->second == "True");
}


bool iniSection::Set(const std::string& sKey, std::string_view sValue, std::string_view sComment)
{
    auto itKey = m_mSectionData.find(sKey);
    if(itKey == m_mSectionData.end())
    {
        m_mSectionData.try_emplace(sKey, sValue);

        return true;
    }
    itKey->second = sValue;
    return false;
}

size_t iniSection::GetNumberOfEntries() const
{
    return m_mSectionData.size();
}

mapIniData::const_iterator iniSection::FindData(const std::string& sKey) const
{
    return m_mSectionData.find(sKey);
}

void iniSection::AddComment(const std::string& sComment)
{

}

void iniSection::Write(std::ofstream& of) const
{
    //write the section name
    of << "[" <<  m_sSectionName << "]\n";

    for(const auto& [sKey, sValue] : m_mSectionData)
    {
        of << sKey << "=" << sValue << "\n";
    }
    of << "\n";
}

bool iniSection::Delete(const std::string& sKey)
{
    m_mSectionData.erase(sKey);
    return true;
}