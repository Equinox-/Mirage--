#ifndef _PLAYERINFO_H
#define _PLAYERINFO_H

#include "limits.h"
#include <SGE.h>
#include "Raknet.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" 
#include "Direction.h"
#include "ItemBase.h"

using namespace SGE;

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class PlayerInfo
{
public:
	PlayerInfo();
	~PlayerInfo();

	void SetUsername(char* username)					{ strcpy(mUsername, username); }
	void SetPosition(SVector2 pos)						{ mPosition = pos; }
	void SetPosition(float x, float y)					{ mPosition = SVector2(x,y); }
	void SetDirection(Direction direction)				{ mDirection = direction; }
	void SetSpriteNumber(int num)						{ mSpriteNumber = num; }

	void AddStr(int str)								{ mStr += str; }
	void AddDex(int dex)								{ mDex += dex; }
	void AddInt(int intStat)							{ mInt += intStat; }
	void AddDef(int def)								{ mDefence += def; }
	void AddAtk(int atk)								{ mAttack += atk; }
	void AddCrit(int crit)								{ mCritChance += crit; }
	void AddHealthBonus(int health)						{ mTotalHealth += health; }
	void AddManaBonus(int mana)							{ mTotalMana += mana; }

	void AddHealth(int health);
	void AddMana(int mana);
	void SetEqiupItem(int type, int itemNumber, int imageNumber);

	char* GetUsername()									{ return mUsername; }

	int GetUsernameLength()								{ int count = 0; for(int a = 0; mUsername[a] != '\0'; ++a) { ++count; } return count;}
	int GetMap()										{ return mMap; }
	int GetSpriteNumber()								{ return mSpriteNumber; }
	RakNetGUID& GetGUID()								{ return mGUID; }
	SVector2 GetPosition()								{ return mPosition; }
	Direction GetDirection()							{ return mDirection; }

	float& GetCurrentHealth()							{ return mCurrentHealth; }
	float& GetTotalHealth()								{ return mTotalHealth; }
	float& GetCurrentMana()								{ return mCurrentMana; }
	float& GetTotalMana()								{ return mTotalMana; }
	float& GetCurrentExp()								{ return mCurrentExp; }
	float& GetTotalExp()								{ return mTotalExp; }

	int GetStr()										{ return mStr; }
	int GetDex()										{ return mDex; }
	int GetInt()										{ return mInt; }

	int GetAttack()										{ return mAttack; }
	int GetDefence()									{ return mDefence; }
	int GetCrit()										{ return mCritChance; }
	int GetGold()										{ return mGold; }
	int GetEqiupItemImage(int slot);

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