#pragma once

#ifndef INCLUDED_CHARACTER_H
#define INCLUDED_CHARACTER_H

//====================================================================================================
// Includes
//====================================================================================================
#include <SGE.h>
using namespace SGE;

#include "Map.h"
#include "CharacterState.h"
#include "CharacterWalk.h"
#include "CharacterIdle.h"
#include "CharacterCombat.h"
#include "Direction.h"
//====================================================================================================
// Class Declaration
//====================================================================================================
class Animation;
class Map;
//====================================================================================================
// Enums
//====================================================================================================

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

enum AnimationState
{
    AS_None = -1,
    AS_Walk,
	AS_Idle,
	AS_Combat,
    AS_Max
};

class Character
{
public:
	Character(Map& map, PlayerInfo& playerInfo, Raknet& raknet);	
	~Character();	

	void Load();
	void Unload();
	void Update(float deltaTime);
	void Render(const SVector2& offset);

	SRect GetBoundingBox() const;

	// Accessors
	const SVector2& GetPosition() const							{ return mPosition; }
	const SVector2& GetVelocity() const							{ return mVelocity; }
	const int GetDirection() const								{ return mDirection; }
	const float GetIdleTimer() const							{ return mIdleTimer; }
	bool IsFacingLeft() const               					{ return mFacingLeft; }
	const int GetFrames() const									{ return mFrames; }
	int OnItem() const											{ return mOnItem; }
	int GetOnItemIndex() const									{ return mOnItemIndex; }
	int GetUsernameLength() const								{ return mUsernameLength; }
	int GetTileIndex() const									{ return mTileIndex; }

	// Mutators
	void SetPosition(const SVector2& pos)						{ mPlayerInfo.SetPosition(pos); mPosition = pos; }
	void SetVelocity(const SVector2& vel)						{ mVelocity = vel; }
	void SetFacingLeft(bool facingLeft)     					{ mFacingLeft = facingLeft; }
	void SetDirection(Direction direction)						{ mDirection = direction; mPlayerInfo.SetDirection(direction); }
	void SetOnItem(int onItem)									{ mOnItem = onItem; }
	void SetOnItemIndex(int index)								{ mOnItemIndex = index; }
	void SetTileIndex(int index)								{ mTileIndex = index; }
	void NewPlayerSprite(int num);
	void UpdateSprite()											{ for(int a = 0; a < AS_Max; ++a) { mCharacterStates[a]->Load(); } }
	
	SGE_Font& GetFont()											{ return mFont; }

	void ChangeState(AnimationState as);
    const char* GetCurrentState();

protected:
	Map& mMap;

	SVector2 mPosition;
	SVector2 mVelocity;
	Direction mDirection;

	int mFrames;
	float mIdleTimer;
	bool mJumping;
	bool mFacingLeft;
	CharacterState* mCharacterStates[AS_Max];
    CharacterState* mpCurrentState;
	PlayerInfo& mPlayerInfo;
	
private:
	void CheckCollision(float fSeconds, Map& map);
	bool mInit;
	Raknet& mRaknet;
	float mUpdateServerTimer;
	float mPlayerPositionTime;
	bool mUpdateResult;
	int mOnItem;
	int mOnItemIndex;
	int mUsernameLength;
	int mTileIndex;
	SGE_Font mFont;
};
#endif