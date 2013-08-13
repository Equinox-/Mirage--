#ifndef _PLAYER_H
#define _PLAYER_H

#include "SVector2.h"
#include <limits.h>
#include <string>
#include "Defines.h"
#include "ActiveMaps.h"
#include "ItemBase.h"

using namespace RakNet;

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================


class Player
{
public:
	Player(const char* username, const char* email, const char* password, const char* ip);
	Player(const char* username);
	~Player();

	void CheckPass();
	void Load();
	void Save();
	bool Create(); // Creates a new player


	void SetLevel(int level)								{ mLevel = level; }
	void SetBanned(bool banned)								{ mBanned = banned; }
	void SetGUID(RakNetGUID guid)							{ mGUID = guid; }
	void SetPosition(SVector2 pos)							{ mPosition = pos; }
	void SetDirection(Direction direction)					{ mDirection = direction; }
	void SetSpriteNumber(int sprite)						{ mSpriteNumber = sprite; }
	void SetItem(int index, int item, int number)			{ mItems[index].first = item; mItems[index].second = number; }
	void AddItem(int item, int number);
	void GetItems(Packet* packet, RakPeerInterface* peer);
	void RemoveItem(int itemNumber);
	void AddHealth(int health);
	void AddMana(int mana);
	
	void ApplyItemBonuses(int itemNumber, int type, Packet* packet, RakPeerInterface* peer, bool eqiup);

	int EquipItem(ItemType slot, int itemNumber, int imageNumber);
	int UneqiupItem(int slot);
	char* GetPassword()										{ return mPassword; }
	char* GetUsername()										{ return mUsername; }
	int GetMap()											{ return mMap; }
	int GetAccess()											{ return mAccessLevel; }
	bool GetBanned()										{ return mBanned; }
	bool HasItem(int itemNumber);
	RakNetGUID GetGUID()									{ return mGUID; }
	Direction GetDirection()								{ return mDirection; }
	SVector2 GetPosition()									{ return mPosition; }


private:
	char mUsername[CHAR_MAX];
	char mPassword[CHAR_MAX];
	char mEmail[CHAR_MAX];
	char mIP[CHAR_MAX];
	RakNetGUID mGUID;

	bool mBanned;
	SVector2 mPosition;
	int mLevel;
	int mMap;
	int mSpriteNumber;
	Direction mDirection;

	float mCurrentHealth;
	float mTotalHealth;
	float mCurrentMana;
	float mTotalMana;
	float mCurrentExp;
	float mTotalExp;

	// first item number, second amount of item
	std::pair <int,int> mItems[50]; 

	int mStr;
	int mDex;
	int mInt;
	
	int mAttack;
	int mDefence;
	int mCritChance;

	int mAccessLevel;

	std::pair <int,int> mEqiupment[9];

	int mGold;
};

#endif
