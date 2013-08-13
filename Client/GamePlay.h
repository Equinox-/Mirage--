#ifndef INCLUDED_GAMEPLAY_H
#define INCLUDED_GAMEPLAY_H

//====================================================================================================
// Includes
//====================================================================================================
#include <SGE.h>
using namespace SGE;

#include "BaseState.h"
#include "GameStates.h"
#include "Raknet.h"
#include "PlayerInfo.h"
#include "Map.h"
#include "Character.h"
#include "OtherPlayers.h"
#include "Commands.h"
#include "HUD.h"

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class GamePlay : public BaseState
{
public:
	GamePlay(Raknet& rak);
	~GamePlay();

	// Implements base state
	virtual void Load();
	virtual void Unload();
	virtual int Update(float deltaTime);
	virtual void Render();

	void UpdateRaknet(float deltaTime);
	
private:
	float mConnectionTimer;
	float mCheckConnectionTime;

	PlayerInfo mPlayerInfo;
	Map mMap;
	Character mCharacter;
	OtherPlayers mOtherPlayers;

	int mScreenWidth;
	int mScreenHeight;

	Commands mCommands;
	ChatBox* mChatBox;
	HUD* mHUD;
};
#endif
