#ifndef INCLUDED_FMOD_H
#define INCLUDED_FMOD_H

//====================================================================================================
// Filename:	CFMOD.h
// Created by:	Peter Chan
// Description:	Singleton class for the audio system. This is a wrapper class for the FMOD sound API.
//			  The singleton can be accessed via a call to CFMOD::Get().
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================
#if __ENABLE_CFMOD
#include "fmod.h"
#endif

//====================================================================================================
// Class Declarations
//====================================================================================================

class CFMOD
{
public:
	// Accessor function for singleton instance
	static CFMOD* Get(void);

	// Destructor
	virtual ~CFMOD(void);

	// Functions to startup and shutdown the audio system
	void Initialize(int iMaxChannels);
	void Terminate(void);

	// Function to update the audio system
	void Update(void);
#if __ENABLE_CFMOD
	// Function to access the FMOD sound system
	FMOD_SYSTEM* FMODSystem(void) const;
#endif
protected:
	// Protected constructor for singleton
	CFMOD(void);

private:
	static CFMOD* s_pInstance;	  // Static instance for singleton
#if __ENABLE_CFMOD
	FMOD_SYSTEM* mpFMODSystem;	 // Pointer to FMOD system
#endif
	bool mInitialized;			 // Init flag
};

#endif // #ifndef INCLUDED_FMOD_H
