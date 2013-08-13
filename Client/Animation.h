#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <SGE.h>
using namespace SGE;
#include "WalkFrames.h"

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class Animation
{
public:
	Animation();
	~Animation();

	//void Update(float deltaTime, Character& character);
	void AnimateSprite(float deltaTime, SGE_Sprite& sprite);
	void UpdateTime(float deltaTime)								{ mTimer += deltaTime; }
	void SetIsAnimating(bool truefalse)								{ mIsAnimating = truefalse; }
	bool GetIsAnimating() const										{ return mIsAnimating; }
	void SetStartFrame(int startFrame)								{ mCurrentStartFrame = startFrame; }
	void SetEndFrame(int endFrame)									{ mCurrentEndFrame = endFrame; }

	const int GetCurrentStartFrame()								{ return mCurrentStartFrame; }	
	const int GetCurrentEndFrame()									{ return mCurrentEndFrame; }	

private:
	float mTimer;
	float mFrameTime;
	int mCurrentStartFrame;
	int mCurrentEndFrame;
	bool mIsAnimating;
};

#endif
