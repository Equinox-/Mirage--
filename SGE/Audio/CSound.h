#ifndef INCLUDED_SOUND_H
#define INCLUDED_SOUND_H

//====================================================================================================
// Filename:	CSound.h
// Created by:	Peter Chan
// Description:	Class for a FMOD sound segment.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "../Resource/CResource.h"

#include "fmod.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class CSound : public CResource
{
public:
	// Constructor
	CSound(void);

	// Destructor
	virtual ~CSound(void);

	//==================================================
	// CResource Implementation
	//==================================================
	// Function to load/unload a new sound
	virtual void Load(const char* pFilename);
	virtual void Unload(void);
	//==================================================

	// Functions to control the playback of the sound
	void Play(bool bLoop = false);
	void Stop(void);
	void Pause(void);
	void Resume(void);
	bool IsPlaying(void) const;
	bool IsPaused(void) const;

	// Functions to get/set volume
	float GetVolume(void) const;
	void SetVolume(float fVolume);
	void AddToVolume(float fVolume);

private:
#if __ENABLE_CFMOD
	FMOD_SOUND* mpSound;		// Pointer to a FMOD sound
	FMOD_CHANNEL* mpChannel;	// Pointer to a FMOD channel
#endif
	float mVolume;

	bool mLoaded;
};

#endif // #ifndef INCLUDED_SOUND_H
