#ifndef _CHARACTERIDLE_H
#define _CHARACTERIDLE_H

//====================================================================================================
// Includes
//====================================================================================================

#include "CharacterState.h"
#include "Animation.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class CharacterIdle : public CharacterState
{
public:
	CharacterIdle(Character* pOwner, PlayerInfo& playerInfo, Raknet& raknet);
	virtual ~CharacterIdle();

	virtual void Load();
	virtual void Unload();
	virtual bool Update(float fSeconds);
	virtual void Render(const SVector2& offset);

	virtual void Enter();
	virtual void Exit();

	virtual SRect GetBoundingBox() const;

private:
	PlayerInfo& mPlayerInfo;
	WalkFrames mFrames;
};

#endif