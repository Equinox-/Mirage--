#ifndef _NPC_H
#define _NPC_H

#include "Defines.h"
#include "NPCData.h"
#include "SVector2.h"
#include "ActiveMaps.h"
#include "CurrentPlayers.h"

class NPC
{
public:
	NPC(ActiveMaps& activeMaps, CurrentPlayers& currentPlayers);
	~NPC();

	void UpdateRaknet(Packet* packet, RakPeerInterface* peer);

private:
	ActiveMaps& mActiveMaps;
	CurrentPlayers& mCurrentPlayers;
};

#endif