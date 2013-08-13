//====================================================================================================
// Filename:	CIniFile.h
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "CIniFile.h"

#include <fstream>
#include <string.h>

//====================================================================================================
// Statics
//====================================================================================================

CIniFile* CIniFile::s_pInstance = NULL;

//====================================================================================================
// Class Definitions
//====================================================================================================

CIniFile* CIniFile::Get(void)
{
	// If we do not yet have an instance created
	if (NULL == s_pInstance)
	{
		// Create a new instance
		s_pInstance = new CIniFile;
	}

	return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

CIniFile::CIniFile(void)
	: mLoaded(false)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

CIniFile::~CIniFile(void)
{
	Unload();
}

//----------------------------------------------------------------------------------------------------

void CIniFile::Load(const char* pFilename)
{
	// Check if we have loaded a file
	if (mLoaded)
	{
		return;
	}

	// Open the file
	std::ifstream file;
	file.open(pFilename);

	// Check if the file is openend properly
	if (!file.is_open())
	{
		return;
	}

	// Keep reading until we reach the end of the file
	std::string line;
	while (!file.eof())
	{
		// Get next line
		std::getline(file, line);

		// Parse line read
		if (line.find("--") == std::string::npos && line.length())
		{
			size_t iEqualPos = line.find('=');
			bool bIsString = (line.find('"') != std::string::npos);
			bool bIsBool = (line.find("true") != std::string::npos) || (line.find("false") != std::string::npos);

			// Add entry
			std::string key = line.substr(0, iEqualPos - 1);
			std::string value = line.substr(iEqualPos + 2);
			if (bIsString)
			{
				// Remove quotes
				value.erase(0, 1);
				value.erase(value.length() - 1, 1);

				// Add string entry
				mStringEntries[key] = value;
			}
			else if (bIsBool)
			{
				// Convert to bool
				bool data = true;
				if (strcmp(value.c_str(), "true") != 0)
					data = false;

				// Add bool entry
				mBoolEntries[key] = data;
			}
			else
			{
				// Convert to float
				float data;
				sscanf(value.c_str(), "%f", &data);

				// Add float entry
				mFloatEntries[key] = data;
			}
		}
	}

	// We are done with the file
	file.close();

	// Set flag
	mLoaded = true;
}

//----------------------------------------------------------------------------------------------------

void CIniFile::Unload(void)
{
	// Check if we have loaded a file
	if (!mLoaded)
	{
		return;
	}

	// Clear everything
	mBoolEntries.clear();
	mFloatEntries.clear();
	mStringEntries.clear();

	// Reset flag
	mLoaded = false;
}

//----------------------------------------------------------------------------------------------------

int CIniFile::GetInt(const char* pTag, int iDefault) const
{
	// Check if we have loaded a file
	if (mLoaded)
	{
		// Search for the entry
		FloatTable::const_iterator itr = mFloatEntries.find(pTag);
		if (itr != mFloatEntries.end())
		{
			return static_cast<int>(itr->second);
		}
	}
		
	return iDefault;
}

//----------------------------------------------------------------------------------------------------

bool CIniFile::GetBool(const char* pTag, bool bDefault) const
{
	// Check if we have loaded a file
	if (mLoaded)
	{
		// Search for the entry
		BoolTable::const_iterator itr = mBoolEntries.find(pTag);
		if (itr != mBoolEntries.end())
		{
			return itr->second;
		}
	}
		
	return bDefault;
}

//----------------------------------------------------------------------------------------------------

float CIniFile::GetFloat(const char* pTag, float fDefault) const
{
	// Check if we have loaded a file
	if (mLoaded)
	{
		// Search for the entry
		FloatTable::const_iterator itr = mFloatEntries.find(pTag);
		if (itr != mFloatEntries.end())
		{
			return itr->second;
		}
	}

	return fDefault;
}

//----------------------------------------------------------------------------------------------------

const char* CIniFile::GetString(const char* pTag, const char* pDefault) const
{
	// Check if we have loaded a file
	if (mLoaded)
	{
		// Search for the entry
		StringTable::const_iterator itr = mStringEntries.find(pTag);
		if (itr != mStringEntries.end())
		{
			return itr->second.c_str();
		}
	}

	return pDefault;
}
