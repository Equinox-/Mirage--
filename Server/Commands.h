#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "CurrentPlayers.h"

class Commands
{
public:
	Commands(CurrentPlayers& currentPlayers);
	~Commands();

	void UpdateRaknet(Packet* packet, RakPeerInterface* peer);

private:
	CurrentPlayers& mCurrentPlayers;
};

#endif