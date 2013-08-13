//====================================================================================================
// Filename:	CFMOD.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "CFMOD.h"

#include "../Core/CLog.h"

//====================================================================================================
// Statics
//====================================================================================================

CFMOD* CFMOD::s_pInstance = 0;

//====================================================================================================
// Class Definitions
//====================================================================================================

CFMOD* CFMOD::Get(void) {
	// If we do not yet have an instance created
	if (0 == s_pInstance) {
		// Create a new instance
		s_pInstance = new CFMOD;
	}

	return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

CFMOD::CFMOD(void) :
		//mpFMODSystem(0),
		mInitialized(false) {
	// Empty
}

//----------------------------------------------------------------------------------------------------

CFMOD::~CFMOD(void) {
	// Terminate the audio system
	Terminate();
}

//----------------------------------------------------------------------------------------------------

void CFMOD::Initialize(int iMaxChannels) {
	// Check if we have already initialized the system
	if (mInitialized) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING,
				"[FMOD] System already initialized.");
		return;
	}

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[FMOD] Initializing...");
#if __ENABLE_CFMOD
	// Create FMOD system
	if (FMOD_OK != FMOD_System_Create(&mpFMODSystem)) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_ERROR,
				"[FMOD] Failed to create FMOD system.");
		return;
	}

	// Initialize FMOD system
	if (FMOD_OK
			!= FMOD_System_Init(mpFMODSystem, iMaxChannels, FMOD_INIT_NORMAL,
					0)) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_ERROR,
				"[FMOD] Failed to initialize FMOD system.");
		return;
	}
#endif
	// Set flag
	mInitialized = true;

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[FMOD] System initialized.");
}

//----------------------------------------------------------------------------------------------------

void CFMOD::Terminate(void) {
	// Check if we have already terminated the manager
	if (!mInitialized) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING,
				"[FMOD] System already terminated.");
		return;
	}

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[FMOD] Terminating...");
#if __ENABLE_CFMOD
	// Release FMOD
	if (0 != mpFMODSystem) {
		FMOD_System_Release (mpFMODSystem);
		mpFMODSystem = 0;
	}
#endif
	// Set flag
	mInitialized = false;

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[FMOD] System terminated");
}

//----------------------------------------------------------------------------------------------------

void CFMOD::Update(void) {
	// Check if we have already initialized the system
	if (!mInitialized) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING,
				"[FMOD] Failed to update audio. System not initialized.");
		return;
	}
#if __ENABLE_CFMOD
	// Update FMOD
	FMOD_System_Update (mpFMODSystem);
#endif
}

//----------------------------------------------------------------------------------------------------
#if __ENABLE_CFMOD
FMOD_SYSTEM* CFMOD::FMODSystem(void) const {
	// Check if we have already initialized the system
	if (!mInitialized) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING,
				"[FMOD] Failed to access audio. System not initialized.");
		return 0;
	}

	return mpFMODSystem;
}
#endif
