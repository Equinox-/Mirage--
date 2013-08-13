#ifndef INCLUDED_TIMER_H
#define INCLUDED_TIMER_H

//====================================================================================================
// Filename:	CTimer.h
// Created by:	Peter Chan
// Description:	Singleton class for hi-res timer. It tracks the time elapsed between each Update()
//			  call and the total number of times Update() is called every second. The singleton can
//			  be accessed via a call to CTimer::Get().
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <time.h>

//====================================================================================================
// Class Declarations
//====================================================================================================

class CTimer
{
public:
	// Accessor function for singleton instance
	static CTimer* Get(void);

public:
	// Function to startup and shutdown the timer
	void Initialize(void);
	void Terminate(void);

	// Function to update the timer can calculate the new elapsed time
	void Update(void);

	// Function to get the elapsed time since last frame
	float GetElapsedSeconds(void) const;

	// Function to get the frame per second
	float GetFPS(void) const;

protected:
	// Protected constructor for singleton
	CTimer(void);

private:
	static CTimer* s_pInstance;		 // Static instance for singleton

	// http://msdn2.microsoft.com/en-us/library/aa383713.aspx
	//LARGE_INTEGER   mTicksPerSecond;   // System clock frequency
	clock_t   mLastTick;
	clock_t   mCurrentTick;

	float   mElapsedSeconds;		   // Time passed since the last call to update
	float   mFPS;					  // Calculated frame per second

	bool	mInitialized;			  // Init flag
};

#endif // #ifndef INCLUDED_TIMER_H
