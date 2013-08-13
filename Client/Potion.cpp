#include "Potion.h"
#include <stdio.h>

Potion::Potion()
:mHealthAmount(0)
,mManaAmount(0)
,mEnergyAmount(0)
{
	for(int a = 0; a < 1000; ++a)
	{
		mDescription[a] = NULL;
	}
}

Potion::~Potion()
{

}

void Potion::Init()
{

}



void Potion::UseItem(PlayerInfo& playerInfo, Raknet& raknet)
{
	playerInfo.AddHealth(mHealthAmount);
	playerInfo.AddMana(mManaAmount);

	// Add Health
	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_ADD_HEALTH);
		bsOut.Write(mHealthAmount);
		raknet.mPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,raknet.mServerAddress,false);
	}

	// Add Mana
	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_ADD_MANA);
		bsOut.Write(mManaAmount);
		raknet.mPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,raknet.mServerAddress,false);
	}

	RemoveItem(raknet);

}

void Potion::GetData(BitStream& bsIn)
{
	int healthAmount = 0;
	int manaAmount = 0;
	bsIn.Read(healthAmount);
	bsIn.Read(manaAmount);

	SetHealth(healthAmount);
	SetMana(manaAmount);
}