#ifndef _POTION_H
#define _POTION_H

#include "ItemBase.h"

class Potion : public ItemBase
{
public:
	Potion();
	~Potion();

	virtual void Init();
	virtual void SendData(BitStream& bsOut);

	void SetHealth(int health)					{ mHealthAmount = health; }
	void SetMana(int mana)						{ mManaAmount = mana; }
	void SetEnergy(int energy)					{ mEnergyAmount = energy; }

protected:
	int mHealthAmount;
	int mManaAmount;
	int mEnergyAmount;
};

#endif