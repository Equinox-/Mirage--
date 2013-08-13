#ifndef _PLAYERPOSITIONS_H
#define _PLAYERPOSITIONS_H

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" 
#include "CurrentPlayers.h"

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class PlayerPosition
{
public:
	PlayerPosition(CurrentPlayers& currentPlayers);
	~PlayerPosition();

	void Update();
	void UpdateRaknet(Packet* packet, RakPeerInterface* peer);
	void NewPlayer(int map, Player& player, Packet* packet, RakPeerInterface* peer);

private:
	CurrentPlayers& mCurrentPlayers;
};

#endif