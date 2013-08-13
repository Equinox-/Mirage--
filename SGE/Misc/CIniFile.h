#ifndef INCLUDED_INIFILE_H
#define INCLUDED_INIFILE_H

//====================================================================================================
// Filename:	CIniFile.h
// Created by:	Peter Chan
// Description:	Singleton class for the init file. It provides an interface for data query via string
//			  tags. Internally it contains three stl maps of string/value pairs: str/float,
//			  str/bool and str/str. The singleton can be accessed via a call to CIniFile::Get().
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <map>
#include <string>

//====================================================================================================
// Class Declarations
//====================================================================================================

class CIniFile
{
public:
	// Accessor function for singleton instance
	static CIniFile* Get(void);

	// Destructor
	virtual ~CIniFile(void);

	// Functions to Load/Unload the init file
	void Load(const char* pFilename);
	void Unload(void);

	// Functions to get data entry by string tag
	int GetInt(const char* pTag, int iDefault = 0) const;
	bool GetBool(const char* pTag, bool bDefault = false) const;
	float GetFloat(const char* pTag, float fDefault = 0.0f) const;
	const char* GetString(const char* pTag, const char* pDefault = "") const;

protected:
	// Protected constructor for singleton
	CIniFile(void);

private:
	// Typedefs
	typedef std::map<std::string, bool> BoolTable;
	typedef std::map<std::string, float> FloatTable;
	typedef std::map<std::string, std::string> StringTable;

	// Members
	static CIniFile* s_pInstance;   // Static instance for singleton

	BoolTable mBoolEntries;		// A hash table of string/bool pairs
	FloatTable mFloatEntries;	  // A hash table of string/float pairs
	StringTable mStringEntries;	// A hash table of string/string pairs

	bool mLoaded;				  // Loaded flag
};

#endif // #ifndef INCLUDED_INIFILE_H