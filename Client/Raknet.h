#ifndef _RAKNET_H
#define _RAKNET_H

#include <stdio.h>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" 

#include <SGE.h>
using namespace SGE;
using namespace RakNet;

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================


enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
	ID_GET_NEWS,
	ID_NEW_ACCOUNT,
	ID_LOGIN,
	ID_LOGIN_WRONG_PASS,
	ID_ACCOUNT_TAKEN,
	ID_STILLCONNECTED,
	ID_CHATMESSAGE,
	ID_GET_PLAYER_INFO,
	ID_MAP_DATA,
	ID_UPDATE_POSITION,
	ID_GET_ACTIVE_PLAYERS,
	ID_GET_PLAYER,
	ID_PICKUP_ITEM,
	ID_ITEM_TAKEN,
	ID_GET_ITEMS,
	ID_REMOVE_ITEM,
	ID_ADD_HEALTH,
	ID_ADD_MANA,
	ID_DROP_ITEM,
	ID_SET_SPRITE,
	ID_SET_SPRITE_NAME,
	ID_USER_BANNED,
	ID_EQIUP_ITEM,
	ID_UNEQIUP_ITEM,
	ID_ITEM_BONUS,
	ID_GET_NPC_INFO,
	ID_NPC_POSITION,
	ID_NPC_NEW_PATH,
	ID_NPC_HOST,
	ID_NPC_STOP,
	ID_PLAYER_LEFT
};

class Raknet
{
public:
	Raknet();
	~Raknet();
	void Connect();
	void Disconnect();


public:
	RakPeerInterface* mPeer;
	SocketDescriptor mSD;
	Packet* mPacket;
	SystemAddress mServerAddress;
	RakNetGUID mClientID;

	bool mConnected;
	const char* mIP;
};

#endif