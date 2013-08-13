#ifndef _CURRENTPLAYERS_H
#define _CURRENTPLAYERS_H

#include <list>
#include "Player.h"
#include <string>
#include "Defines.h"
#include <time.h> 
#include "Data.h"
#include "SVector2.h"
#include "ActiveMaps.h"

class ActiveMaps;
class Player;
using namespace RakNet;
using namespace std;

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class CurrentPlayers
{
public:
	CurrentPlayers(Data& data, RakPeerInterface* peer);
	~CurrentPlayers();

	bool Login(const char* username, const char* password, RakNetGUID guid);
	void Disconnect(RakNetGUID guid);
	void ConnectionUpdate();
	void SendPlayerInfo(RakNetGUID guid);
	void SavePlayers();
	void ItemDropped(Packet* packet, RakPeerInterface* peer, int map, int imageNumber, int index);
	void SetActiveMaps(ActiveMaps* activeMaps)		{ mActiveMaps = activeMaps; }
	void SendNPCStop(Packet* packet, RakPeerInterface* peer, int arrayIndex, bool stop, int map);

	void Update();
	Player* GetPlayer(RakNetGUID& guid);
	Player* GetPlayer(char* name);
	list<Player>& GetList()							{ return mPlayers; }

	void SendItemTaken(int itemIndex, int map);
	void SendNewPath(int map, int npcSlot, int endIndex, SVector2 position);

	int GetPlayersOnline()							{ return mPlayers.size(); }


private:
	list<Player> mPlayers;
	clock_t mStartTime;
	clock_t mTimePassed;
	double mUpdateTimer;

	Data& mData;
	RakPeerInterface* mPeer;
	ActiveMaps* mActiveMaps;
};

#endif