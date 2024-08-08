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
#pragma once

#include <map>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "inisection.h"
#include <memory>
#include <filesystem>
#include <list>

using mSections = std::map<std::string, std::shared_ptr<iniSection>, ci_less>;

/**
*   @brief simple class to read and write ini files
*   @author Matthew Martin
*   @version 0.1
*   @date 2008
*
*   iniManager reads in an ini file and stores the sections, keys and values
*
**/
class iniManager
{
    public:
        /** @brief Constructor
        **/
        iniManager();

        /** @brief Destructor
        **/
        ~iniManager();

        /** @brief Reads in an inifile
        *   @param sFilename the path and filename of the ini file to read in
        *   @return <i>bool<i> true on success
        **/
        bool Read(const std::filesystem::path& filepath);

        bool ReRead();

        /** @brief Get the string value for the specified key in the specified section.
        *   @param sSection the section of the ini file the key live is
        *   @param sKey the key you want to retrieve the value of
        *   @param sDefault the default value of the key (this is returned if the key is not set)
        *   @return <i>string</i> the value of the key
        **/
        const std::string& Get(const std::string& sSection, const std::string& sKey, const std::string& sDefault) const;
        /** @brief Get the ini value for the specified key in the specified section.
        *   @param sSection the section of the ini file the key live is
        *   @param sKey the key you want to retrieve the value of
        *   @param nDefault the default value of the key (this is returned if the key is not set)
        *   @return <i>long</i> the value of the key
        **/
        long Get(const std::string& sSection, const std::string& sKey, long nDefault) const;

        /** @brief Get the double value for the specified key in the specified section.
        *   @param sSection the section of the ini file the key live is
        *   @param sKey the key you want to retrieve the value of
        *   @param dDefault the default value of the key (this is returned if the key is not set)
        *   @return <i>double</i> the value of the key
        **/
        double Get(const std::string& sSection, const std::string& sKey, double dDefault) const;

        /** @brief Get the boolean value for the specified key in the specified section.
        *   @param sSection the section of the ini file the key live is
        *   @param sKey the key you want to retrieve the value of
        *   @param bDefault the default value of the key (this is returned if the key is not set)
        *   @return <i>bool</i> the value of the key
        **/
        bool GetBool(const std::string& sSection, const std::string& sKey, bool bDefault) const;

        /** @brief Writes the stored sections and keys into an ini file
        *   @param sFilename the path and filename of the ini file to write to
        *   @return <i>bool<i> true on success
        **/
        bool Write(const std::filesystem::path& filepath);

        bool Write();

        /** @brief  Creates or returns a section to store keys and values in
        *   @param sSectionName the name of the sectino
        *   @return <i>iniSection*</i> pointer to the created section.
        **/
        std::shared_ptr<iniSection> CreateSection(const std::string& sSectionName);

        /** @brief Sets the value of the specified key. Creating both the key and section if needed
        *   @param sSectionName the name of the section
        *   @param sKey the key to set the value of
        *   @param sValue the value to set the key
        *   @return <i>void</i>
        **/
        void Set(const std::string& sSectionName, const std::string& sKey, const std::string& sValue);

        /** @brief Sets the value of the specified key. Creating both the key and section if needed
        *   @param sSectionName the name of the section
        *   @param sKey the key to set the value of
        *   @param sValue the value to set the key
        *   @return <i>void</i>
        **/
        void Set(const std::string& sSectionName, const std::string& sKey, long nValue);

        void Set(const std::string& sSectionName, const std::string& sKey, double dValue);

        void SetBool(const std::string& sSectionName, const std::string& sKey, bool bValue);

        /** @brief Returns the number of entries in a given section
        *   @param sSectionName the name of the section
        *   @return <i>size_t</i> the number of entries
        **/
        size_t GetNumberOfSectionEntries(const std::string& sSectionName) const;


        const mSections& GetSections() const { return m_mSections;}

        /** @brief Get a pointer to an iniSection with name sSectionName
        *   @param sSectionName the section to find
        *   @return <i>iniSection*</i> pointer to the iniSection object that encapsulates that section
        **/
        std::shared_ptr<iniSection> GetSection(const std::string& sSectionName) const;

        /** @brief Deletes a section if it exists
        *   @param sSectionName
        *   @return <i>bool</i> true if section is found and deleted
        **/
        bool Delete(const std::string& sSectionName);

        bool Delete(const std::string& sSectionName, const std::string& sKey);

        const std::filesystem::path& GetPath() const { return m_filepath;}
    private:
        bool ReadStream(std::stringstream& isstr);
        mSections::iterator MakeSection(const std::string& sLine);
        void StoreData(std::shared_ptr<iniSection> pSection, const std::string& sLine);

        mSections m_mSections;


        std::ifstream m_if;
        std::ofstream m_of;
        std::filesystem::path m_filepath;

        void DeleteSections();

};


