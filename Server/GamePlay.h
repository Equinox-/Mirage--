#ifndef _GAMEPLAY_H
#define _GAMEPLAY_H

#include "Chat.h"
#include "Defines.h"
#include "CurrentPlayers.h"
#include "MapLoad.h"
#include "SavePlayers.h"
#include "PlayerPosition.h"
#include "ActiveMaps.h"
#include "Commands.h"
#include "ItemEqiups.h"
#include "NPC.h"

using namespace RakNet;

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class GamePlay
{
public:
	GamePlay(CurrentPlayers& currentPlayers, Data& data);
	~GamePlay();

	void Init();
	void Update();
	void UpdateRaknet(Packet* packet, RakPeerInterface* peer);

	ActiveMaps* GetActiveMaps()										{ return &mActiveMaps; }
private:
	Chat mChat;
	CurrentPlayers& mCurrentPlayers;
	SavePlayers mSavePlayers;
	Data& mData;
	PlayerPosition mPlayerPosition;
	ActiveMaps mActiveMaps;
	Commands mCommands;
	ItemEqiups mItemEqiups;
	NPC mNPC;
	bool mInit;
};
#endif