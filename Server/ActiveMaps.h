#ifndef _ActiveMapItems_H
#define _ActiveMapItems_H

#include <vector>
#include "GroundTile.h"
#include "Defines.h"
#include "Potion.h"
#include "CurrentPlayers.h"
#include "ItemHelper.h"
#include "MapData.h"
#include <assert.h>

using namespace RakNet;

class CurrentPlayers;
class LoadMap;
class ActiveMaps
{
public:
	ActiveMaps(CurrentPlayers& currentPlayers);
	~ActiveMaps();
	void AddMap(MapData* map)								{ mMaps.push_back(map); }
	void RemoveMap(int id);
	bool DoesMapExist(int id);

	void UpdateRaknet(Packet* packet, RakPeerInterface* peer);


	MapData* GetMap(int map); 

private:
	std::vector<MapData*> mMaps;
	CurrentPlayers& mCurrentPlayers;
};

#endif