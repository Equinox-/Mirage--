#ifndef _PLAYER_H
#define _PLAYER_H

#include "Animation.h"
#include <SGE.h>
#include "PlayerInfo.h"
#include <SGE.h>
#include "Animation.h"

using namespace SGE;

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class Player
{
public:
	Player(PlayerInfo playerInfo);
	~Player();

	void Init();
	void Update(float deltaTime);
	void Render(SVector2& offset);

	void SetNewPosition(SVector2 position)						{ mNewPosition = position; }
	SVector2 GetPosition()										{ return mPlayerInfo.GetPosition(); }
	PlayerInfo mPlayerInfo;
	SGE_Sprite mSprite;

private:

	bool mInit;
	Animation mAnimation;
	WalkFrames mFrames;

	SVector2 mNewPosition;
	Direction mOldDirection;
};

#endif