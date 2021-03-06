//====================================================================================================
// Filename:	CTimer.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "CTimer.h"

#include "CLog.h"

#if __linux
#include <string.h>
#include <sys/time.h>
#endif

//====================================================================================================
// Statics
//====================================================================================================

CTimer* CTimer::s_pInstance = NULL;

//====================================================================================================
// Class Definitions
//====================================================================================================

CTimer* CTimer::Get(void) {
	// If we do not yet have an instance created
	if (NULL == s_pInstance) {
		// Create a new instance
		s_pInstance = new CTimer;
	}

	return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

CTimer::CTimer(void) :
		mElapsedSeconds(0.0f), mFPS(0.0f), mInitialized(false) {
#if __linux
	//Init mCurrTick and mLastTick?
#else
	mCurrTick = 0;
	mLastTick = 0;
#endif
}

//----------------------------------------------------------------------------------------------------

void CTimer::Initialize(void) {
	// Check if we have already initialized the timer
	if (mInitialized) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING,
				"[Timer] Timer already initialized.");
		return;
	}

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[Timer] Initializing...");

#if __linux
	gettimeofday(&mCurrentTick, NULL);
	memcpy(&mLastTick, &mCurrentTick, sizeof(timeval));
	memcpy(&mInitTick, &mCurrentTick, sizeof(timeval));
#else
	mCurrentTick = clock();
	mLastTick = mCurrentTick;
#endif

	// Set flag
	mInitialized = true;

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[Timer] Timer initialized.");
}

float CTimer::GetCurrentTick() {
#if __linux
	timeval temp;
	gettimeofday(&temp, NULL);
	return ((float) (temp.tv_sec - mInitTick.tv_sec)
			+ ((float) (temp.tv_usec - mInitTick.tv_usec) / 1000000.0));
#else
	return ((float) clock()) / (float) CLOCKS_PER_SEC;
#endif
}
//----------------------------------------------------------------------------------------------------

void CTimer::Terminate(void) {
	// Check if we have already terminated the timer
	if (!mInitialized) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING,
				"[Timer] Timer already terminated.");
		return;
	}

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[Timer] Terminating...");

	// Reset values
	mElapsedSeconds = 0.0f;
	mFPS = 0.0f;

	// Set flag
	mInitialized = false;

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[Timer] Timer terminated.");
}

//----------------------------------------------------------------------------------------------------

void CTimer::Update(void) {
	// Make sure the timer is initialized
	if (!mInitialized) {
		CLog::Get()->Write(ELogMessageType_ERROR,
				"[Timer] Timer not initialized.");
		return;
	}

	// Get the current tick count
#if __linux
	gettimeofday(&mCurrentTick, NULL);
	mElapsedSeconds = ((float) (mCurrentTick.tv_sec - mLastTick.tv_sec))
			+ (((float) mCurrentTick.tv_usec) / 1000000.0)
			- (((float) mLastTick.tv_usec) / 1000000.0);
#else
	mCurrentTick = clock();
	// Calculate the elapsed time
	mElapsedSeconds = static_cast<float>(mCurrentTick - mLastTick)
	/ (float) CLOCKS_PER_SEC;
#endif
	if (mElapsedSeconds < 0.0) {
		mElapsedSeconds = 0.001;
	}

	// Update the last tick count
#if __linux
	memcpy(&mLastTick, &mCurrentTick, sizeof(timeval));
#else
	mLastTick = mCurrentTick;
#endif

	static float s_FrameSinceLastSecond = 0.0f;
	static float s_AccumulatedTime = 0.0f;

	// Calculate the FPS
	s_AccumulatedTime += mElapsedSeconds;
	s_FrameSinceLastSecond += 1.0f;

	// Update FPS is one second has passed
	if (s_AccumulatedTime >= 1.0f) {
		mFPS = s_FrameSinceLastSecond / s_AccumulatedTime;
		s_AccumulatedTime = 0.0f;
		s_FrameSinceLastSecond = 0.0f;
	}
}

//----------------------------------------------------------------------------------------------------

float CTimer::GetElapsedSeconds(void) const {
	return mElapsedSeconds;
}

//----------------------------------------------------------------------------------------------------

float CTimer::GetFPS(void) const {
	return mFPS;
}
