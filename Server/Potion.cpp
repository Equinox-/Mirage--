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

void Potion::SendData(BitStream& bsOut)
{
	bsOut.Write(mHealthAmount);
	bsOut.Write(mManaAmount);
}