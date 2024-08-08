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
#include "inimanager.h"
#include "inisection.h"
#include <sstream>
#include "log.h"


iniManager::iniManager()=default;

iniManager::~iniManager()
{
	DeleteSections();
}

void iniManager::DeleteSections()
{
	m_mSections.clear();
}

bool iniManager::ReRead()
{
    if(m_filepath.empty())
    {
        return false;
    }
    return Read(m_filepath);
}

/*!
    \fn iniManager::ReadIniFile(const string& sFilename)
 */
bool iniManager::Read(const std::filesystem::path& filepath)
{
    m_filepath = filepath;

	DeleteSections();

    std::ifstream ifFile;

    //attempt to open the file
    ifFile.open(m_filepath,std::ios::in);
    if(!ifFile.is_open())
    {
        return false;
    }

    std::stringstream isstr;
    isstr << ifFile.rdbuf();
    ifFile.close();


    return ReadStream(isstr);
}

bool iniManager::ReadStream(std::stringstream& isstr)
{
    std::string sLine;
    std::string sTag;
    std::string sData;

    unsigned long nLine(0);
    //read in each line
    auto itSection = m_mSections.end();
    while(std::getline(isstr, sLine))
    {
        ++nLine;
        // if the line starts with a [ then its the start of a section
        if(sLine[0] == '[')
        {
            itSection = MakeSection(sLine);
        }
        if(sLine[0] == '#' || sLine[0] == ';')
        {
            if(itSection != m_mSections.end())
            {
                itSection->second->AddComment(sLine);
            }
        }
        else if(sLine.size() < 2)
        {
            if(itSection != m_mSections.end())
            {
                itSection->second->AddComment("");
            }
        }
        else if(itSection != m_mSections.end())
        {
            StoreData(itSection->second, sLine);
        }
    }

    return true;
}

void iniManager::StoreData(std::shared_ptr<iniSection> pSection, const std::string& sLine) 
{
    size_t nEqualPos = std::string::npos;
    size_t nCommentPos = 0;
    std::string sKey;
    std::string sData;
    sKey.reserve(sLine.length());
    sData.reserve(sLine.length());

    std::string sInlineComment;
    for(;nCommentPos < sLine.length(); nCommentPos++)
    {
        sInlineComment.clear();

        if(sLine[nCommentPos] == '#' || sLine[nCommentPos] == ';')
        {
            sInlineComment = sLine.substr(nCommentPos);
            break;
        }
        else if(sLine[nCommentPos] == '=' && nEqualPos == std::string::npos)
        {
            nEqualPos = nCommentPos;
        }
        else if(nEqualPos == std::string::npos)
        {
            sKey += sLine[nCommentPos];
        }
        else
        {
            sData += sLine[nCommentPos];
        }
    }
    if(nEqualPos != std::string::npos)
    {
        pSection->Set(sKey, sData, sInlineComment);
    }
}

std::shared_ptr<iniSection> iniManager::GetSection(const std::string& sSectionName) const
{
    if(auto it = m_mSections.find(sSectionName); it != m_mSections.end())
    {
        return it->second;
    }
    return nullptr;
}

mSections::iterator iniManager::MakeSection(const std::string& sLine)
{
    std::string sSection;
    sSection.reserve(sLine.length());

    for(size_t i = 1; i < sLine.length(); i++)
    {
        if(sLine[i] == ']')
            break;
        sSection+=sLine[i];
    }
    //get the name of the section
    return m_mSections.try_emplace(sSection, std::make_shared<iniSection>(sSection)).first;
}

/*!
    \fn iniManager::GetIniString(const string& sSection, const string& sKey, const string& sDefault)
 */
const std::string& iniManager::Get(const std::string& sSection, const std::string& sKey, const std::string& sDefault) const
{
    //does the section exist?
	if(auto it = m_mSections.find(sSection); it!=m_mSections.end())
	{
        return it->second->Get(sKey,sDefault);    
    }
    return sDefault;
}

/*!
    \fn iniManager::GetIniInt(const string& sSection, const string& sKey, int nDefault)
 */
long iniManager::Get(const std::string& sSection, const std::string& sKey, long nDefault) const
{
    //does the section exist?
	if(auto it = m_mSections.find(sSection); it!=m_mSections.end())
	{
        return it->second->Get(sKey,nDefault);
    }
    return nDefault;
}

/*!
    \fn iniManager::GetIniDouble(const string& sSection, const string& sKey, double dDefault)
 */
double iniManager::Get(const std::string& sSection, const std::string& sKey, double dDefault) const
{
    //does the section exist?
	if(auto it = m_mSections.find(sSection); it!=m_mSections.end())
	{
        return it->second->Get(sKey,dDefault);
    }
    return dDefault;
}

bool iniManager::GetBool(const std::string& sSection, const std::string& sKey, bool bDefault) const
{
    //does the section exist?
	
	if(auto it = m_mSections.find(sSection); it!=m_mSections.end())
	{
        return it->second->Get(sKey, bDefault);
    }	
    return bDefault;
}

bool iniManager::Write()
{
    return Write(m_filepath);
}

bool iniManager::Write(const std::filesystem::path& filepath)
{
	m_filepath = filepath;

    //Close file if open
	if(m_of.is_open())
    {
        m_of.close();
	}

	//attempt to open the file

	m_of.open(m_filepath);
	if(!m_of.is_open())
	{
		return false;
	}

	m_of.clear();

    for(const auto& [sSection, pSection] : m_mSections)
    {
        pSection->Write(m_of);
    }
    //Close the file again
	if(m_of.is_open())
	{
	    m_of.close();
	}

    return true;

}

std::shared_ptr<iniSection> iniManager::CreateSection(const std::string& sSectionName)
{
    auto itSection =m_mSections.find(sSectionName);
    if(itSection == m_mSections.end())
    {
        itSection  = m_mSections.try_emplace(sSectionName,std::make_shared<iniSection>(sSectionName)).first;
    }
    return itSection->second;
}

void iniManager::Set(const std::string& sSectionName, const std::string& sKey, const std::string& sValue)
{
    auto pSection = CreateSection(sSectionName);
    pSection->Set(sKey,sValue);
    
}

void iniManager::Set(const std::string& sSectionName, const std::string& sKey, long nValue)
{
    std::stringstream ss;
    ss << nValue;
    Set(sSectionName, sKey, ss.str());
}

void iniManager::Set(const std::string& sSectionName, const std::string& sKey, double dValue)
{
    std::stringstream ss;
    ss << dValue;
    Set(sSectionName, sKey, ss.str());
}

void iniManager::SetBool(const std::string& sSectionName, const std::string& sKey, bool bValue)
{
    Set(sSectionName, sKey, bValue ? std::string("1") : std::string("0"));
}

size_t iniManager::GetNumberOfSectionEntries(const std::string& sSectionName) const
{
	if(auto it = m_mSections.find(sSectionName); it!= m_mSections.end())
    {
        return it->second->GetNumberOfEntries();
    }
    return 0;
}

bool iniManager::Delete(const std::string& sSectionName)
{
    if(GetSection(sSectionName))
    {
        m_mSections.erase(sSectionName);       
        return true;
    }
    return false;
}

bool iniManager::Delete(const std::string& sSectionName, const std::string& sKey)
{
    if(auto pSection = GetSection(sSectionName);pSection && pSection->Delete(sKey))
    {
        return true;
    }    
    return false;
}
