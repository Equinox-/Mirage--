#include "Helmet.h"

Helmet::Helmet()
:mStrBonus(0)
,mDefBonus(0)
,mDexBonus(0)
,mIntBonus(0)
,mAtkBonus(0)
,mCritBonus(0)
,mHealthBonus(0)
,mManaBonus(0)
,mDefence(0)
,mLevelReq(0)
,mStrReq(0)
,mDefReq(0)
,mDexReq(0)
,mIntReq(0)
,mAtkReq(0)
{

}

Helmet::~Helmet()
{

}

void Helmet::Init()
{

}


void Helmet::GetData(BitStream& bsIn)
{
	bsIn.Read(mStrBonus);
	bsIn.Read(mDefBonus);
	bsIn.Read(mDexBonus);
	bsIn.Read(mDefBonus);
	bsIn.Read(mDexBonus);
	bsIn.Read(mIntBonus);
	bsIn.Read(mAtkBonus);
	bsIn.Read(mCritBonus);
	bsIn.Read(mHealthBonus);
	bsIn.Read(mManaBonus);
	bsIn.Read(mDefence);
	bsIn.Read(mLevelReq);
	bsIn.Read(mStrReq);
	bsIn.Read(mDefReq);
	bsIn.Read(mDexReq);
	bsIn.Read(mIntReq);
	bsIn.Read(mAtkReq);
}

void Helmet::UseItem(PlayerInfo& playerInfo, Raknet& raknet)
{
	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_EQIUP_ITEM);
		bsOut.Write(mItemNumber);
		bsOut.Write(mImageNumber);
		bsOut.Write(helmet);
		raknet.mPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,raknet.mServerAddress,false);
	}

	RemoveItem(raknet);
}