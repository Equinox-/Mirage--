#include "Weapon.h"

Weapon::Weapon()
:mStrBonus(0)
,mDefBonus(0)
,mDexBonus(0)
,mIntBonus(0)
,mAtkBonus(0)
,mCritBonus(0)
,mHealthBonus(0)
,mManaBonus(0)
,mDamage(0)
,mSpeed(0)
,mLevelReq(0)
,mStrReq(0)
,mDefReq(0)
,mDexReq(0)
,mIntReq(0)
,mAtkReq(0)
{

}

Weapon::~Weapon()
{

}

void Weapon::Init()
{

}

void Weapon::SendData(BitStream& bsOut)
{
	bsOut.Write(mStrBonus);
	bsOut.Write(mDefBonus);
	bsOut.Write(mDexBonus);
	bsOut.Write(mDefBonus);
	bsOut.Write(mDexBonus);
	bsOut.Write(mIntBonus);
	bsOut.Write(mAtkBonus);
	bsOut.Write(mCritBonus);
	bsOut.Write(mHealthBonus);
	bsOut.Write(mManaBonus);
	bsOut.Write(mDamage);
	bsOut.Write(mSpeed);
	bsOut.Write(mLevelReq);
	bsOut.Write(mStrReq);
	bsOut.Write(mDefReq);
	bsOut.Write(mDexReq);
	bsOut.Write(mIntReq);
	bsOut.Write(mAtkReq);
}