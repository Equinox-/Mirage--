#ifndef INCLUDED_LOG_H
#define INCLUDED_LOG_H

#include <stdio.h>

//====================================================================================================
// Filename:	CLog.h
// Created by:	Peter Chan
// Description:	Singleton class for logging messages. There are three log message types: standard
//				messages (blue), warnings (yellow), and errors (red). The messages are written to an
//				external HTML file. The singleton can be accessed via a call to CLog::Get().
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

//====================================================================================================
// Enums
//====================================================================================================

enum ELogMessageType
{
	ELogMessageType_MESSAGE,
	ELogMessageType_WARNING,
	ELogMessageType_ERROR,
	ELogMessageType_MAX
};

//====================================================================================================
// Class Declarations
//====================================================================================================

class CLog
{
public:
	// Accessor function for singleton instance
	static CLog* Get(void);

public:
	// Initialize the log
	void Initialize(const char* pFilename = "Log.html");

	// Terminate the log
	void Terminate(void);

	// Write to log
	void Write(ELogMessageType eLogMessageType, const char* pMessage, ...);
	void Write(ELogMessageType eLogMessageType, const char* pMessage, va_list args);

protected:
	// Protected constructor for singleton
	CLog(void);

private:
	static CLog* s_pInstance;   // Static instance for singleton

	char* mpFilename;		  // Name of log file

	bool mInitialized;		 // Init flag
};

#endif // #ifndef INCLUDED_LOG_H
