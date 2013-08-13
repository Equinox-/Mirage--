#ifndef _CHARACTERWALK_H
#define _CHARACTERWALK_H

//====================================================================================================
// Includes
//====================================================================================================

#include "CharacterState.h"
#include "Animation.h"
#include "Raknet.h"
#include "Map.h"

class Animation;
class CharacterState;
class Map;
//====================================================================================================
// Class Declarations
//====================================================================================================

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class CharacterWalk : public CharacterState
{
public:
    CharacterWalk(Character* pOwner, Map& map, PlayerInfo& playerInfo, Raknet& raknet);
    virtual ~CharacterWalk();

    virtual void Load();
    virtual void Unload();
    virtual bool Update(float fSeconds);
    virtual void Render(const SVector2& offset);

    virtual void Enter();
    virtual void Exit();

    virtual SRect GetBoundingBox() const;
	

	bool GetMoved()											{ return mMoved; }

private:
	Map& mMap;
	int mSpriteWalkAnimations;
	WalkFrames mFrames;
	bool mMoved;
};

#endif
