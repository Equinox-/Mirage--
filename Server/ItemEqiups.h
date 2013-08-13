#ifndef _ITEM_EQIUPS_H
#define _ITEM_EQIUPS_H

#include "Defines.h"
#include "CurrentPlayers.h"

class ItemEqiups
{
public:
	ItemEqiups(CurrentPlayers& activePlayers);
	~ItemEqiups();

	void UpdateRaknet(Packet* packet, RakPeerInterface* peer);
	

private:
	CurrentPlayers& mCurrentPlayers;
};

#endif