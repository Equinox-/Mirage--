#ifndef _BASECONNECTION_H
#define _BASECONNECTION_H

#include "Defines.h"
#include "CurrentPlayers.h"

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class BaseConnection
{
public:
	BaseConnection(CurrentPlayers& currentPlayers);
	~BaseConnection();

	void Update(Packet* packet, RakPeerInterface* peer);

private:
	CurrentPlayers& mCurrentPlayers;
	int mActiveConnections;
};

#endif