#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "Raknet.h"
#include "Character.h"
#include "PlayerInfo.h"
#include "Map.h"

class Commands
{
public:
	Commands(Raknet& raknet, PlayerInfo& playerInfo, Character& character, Map& map);
	~Commands();

	void UpdateRaknet();

private:

	Raknet& mRaknet;
	PlayerInfo& mPlayerInfo;
	Character& mCharacter;
	Map& mMap;
};

#endif