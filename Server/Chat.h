#ifndef _CHAT_H
#define _CHAT_H

#include "Defines.h"
#include "CurrentPlayers.h"

using namespace RakNet;

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class Chat
{
public:
	Chat(CurrentPlayers& currentPlayers);
	~Chat();
	void Init();
	void Update(Packet* packet, RakPeerInterface* peer);

private:
	CurrentPlayers& mCurrentPlayers;

};

#endif