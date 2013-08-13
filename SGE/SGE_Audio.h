#ifndef INCLUDED_SGE_AUDIO_H
#define INCLUDED_SGE_AUDIO_H

//====================================================================================================
// Filename:	SGE_Audio.h
// Created by:	Peter Chan
// Description:	Header containing some useful classes for audio playback.
//====================================================================================================

//====================================================================================================
// Forward Declarations
//====================================================================================================

class CSound;

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// Class Declarations
//====================================================================================================

class SGE_Sound
{
public:
	SGE_Sound(void);
	~SGE_Sound(void);

	void Load(const char* pSoundName);
	void Unload(void);

	void Play(bool bLoop = false);
	void Stop(void);

private:
	CSound* mpSound;
};

} // namespace SGE

#endif // #ifndef INCLUDED_SGE_AUDIO_H