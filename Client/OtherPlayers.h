#ifndef _OTHERPLAYERS_H
#define _OTHERPLAYERS_H

#include "Raknet.h"
#include <list>
#include "PlayerInfo.h"
#include <SGE.h>
#include "Player.h"
#include "Map.h"

using namespace SGE;

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class OtherPlayers
{
public:
	OtherPlayers(Raknet& raknet, PlayerInfo& playerInfo);
	~OtherPlayers();

	void Init();
	void Update(float deltaTime);
	void Render(float deltaTime);
	void UpdateRaknet();
	void GetPlayerActivePlayers();
	void RemovePlayer(RakNetGUID& guid);
	void Render(SVector2 offset);

	Player* GetPlayer(char* name);
	Player* GetPlayer(int index)								{ return mPlayers[index]; }

	int GetNumberOfPlayers()									{ return mPlayers.size(); }

private:
Raknet& mRaknet;
std::vector<Player*> mPlayers;
PlayerInfo& mPlayerInfo;
SGE_Font mFont;
};

#endif