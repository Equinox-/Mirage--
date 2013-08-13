#ifndef INCLUDED_SGE_GRAPHICS_H
#define INCLUDED_SGE_GRAPHICS_H

//====================================================================================================
// Filename:	SGE_Graphics.h
// Created by:	Peter Chan
// Description:	Header containing some useful classes for graphics rendering.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <vector>

#include "SGE_Math.h"

//====================================================================================================
// Typedefs
//====================================================================================================

typedef unsigned int COLOR;

//====================================================================================================
// Forward Declarations
//====================================================================================================

class CParticle;
class CSprite;
class CTexture;
class CWireFrame;

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// Class Declarations
//====================================================================================================

class SGE_Sprite
{
public:
	SGE_Sprite(void);
	~SGE_Sprite(void);

	void Load(const char* pTextureName);
	void Add(const char* pTextureName);
	void Unload(void);

	void Update(float fSeconds);
	void Render(void);

	void SetPosition(float x, float y);
	void SetPosition(const SVector2& pos);
	void SetScale(float x, float y, bool scaleAboutCenter = true);
	void SetScale(const SVector2& scale, bool scaleAboutCenter = true);
	void SetScaleAboutCenter(bool scaleAboutCenter);
	void SetRotation(float rotation);
	void SetFlipH(bool bFlip);
	void SetFlipV(bool bFlip);

	void Play(float speed, bool loop);
	void Stop(void);
	void Reset(void);

	int GetWidth(void) const;
	int GetHeight(void) const;

	bool IsFinished(void) const;

	int GetCurrentFrame(void) const;
	void SetCurrentFrame(int iFrame);
	void SetAlpha(int alpha);
	void SetColor(int iRed, int iGreen, int iBlue, int iAlpha);

private:
	std::vector<CTexture*> mpTextures;
	CSprite*				 mpSprite;
};

//----------------------------------------------------------------------------------------------------

class SGE_Emitter
{
public:
	SGE_Emitter(void);
	~SGE_Emitter(void);

	void Load(const char* pEmitterName);
	void Unload(void);

	void Update(float fSeconds);
	void Render(bool bAdditiveBlend = false);

	void Start(float rate, bool burst);
	void Stop(void);

	void SetPosition(float x, float y);
	void SetPosition(const SVector2& pos);

	void SetOffset(float x, float y);
	void SetOffset(SVector2 pos);

private:
	CParticle*	mParticles;
	CSprite*	mpSprite;
	CTexture*	mpTexture;

	// Emission Attributes
	float	mTimer;
	float	mRate;
	float	mInterval;
	float	mEmitted;
	int		mIndex;

	// Attributes set in game
	SVector2	mPosition;
	SVector2	mOffset;
	bool		mFire;
	bool		mBurst;

	// Attributes set from script
	int			mMaxBufferSize;
	SRect		mRegion;
	SVector2	mDirection;
	float		mSpan;
	float		mSpeedMin, mSpeedMax;
	float		mLifetime;
	float		mScaleMin, mScaleMax;
	COLOR		mStartColor, mEndColor;
};

//----------------------------------------------------------------------------------------------------

class SGE_WireFrame
{
public:
	SGE_WireFrame(void);
	SGE_WireFrame(const SGE_WireFrame&);
	~SGE_WireFrame(void);
	void operator=(const SGE_WireFrame&);

	void Render(void);

	void AddLine(float x0, float y0, float x1, float y1);
	void AddLine(const SVector2& start, const SVector2& end);
	void AddRect(float x, float y, float w, float h);
	void AddRect(const SVector2& pos, const SVector2& dim);
	void AddRect(const SRect& rRect);
	void AddCircle(float x, float y, float r);
	void AddCircle(const SVector2& c, float r);
	void AddCircle(const SCircle& rCircle);

	void Purge(void);

	void SetLineWidth(float fLineWidth);
	void SetColor(unsigned int rgb);

private:
	CWireFrame* mWireFrame;
};

} // namespace SGE

#endif // #ifndef INCLUDED_SGE_GRAPHICS_H