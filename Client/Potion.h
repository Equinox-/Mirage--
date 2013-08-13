#ifndef _POTION_H
#define _POTION_H

#include "ItemBase.h"
#include "PlayerInfo.h"

class Potion : public ItemBase
{
public:
	Potion();
	~Potion();

	virtual void Init();
	virtual void GetData(BitStream& bsIn);

	void SetHealth(int health)					{ mHealthAmount = health; }
	void SetMana(int mana)						{ mManaAmount = mana; }
	void SetEnergy(int energy)					{ mEnergyAmount = energy; }
	void UseItem(PlayerInfo& playerInfo, Raknet& raknet);

	int GetHealth()								{ return mHealthAmount; }
	int GetMana()								{ return mManaAmount; }

protected:
	int mHealthAmount;
	int mManaAmount;
	int mEnergyAmount;
};

#endif