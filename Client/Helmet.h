#ifndef _Helmet_H
#define _Helmet_H

#include "ItemBase.h"

class Helmet : public ItemBase
{
public:
	Helmet();
	~Helmet();

	virtual void Init();
	void GetData(BitStream& bsIn);

	void SetStrBonus(int bonus)				{ mStrBonus = bonus; }
	void SetDefBonus(int bonus)				{ mStrBonus = bonus; }
	void SetDexBonus(int bonus)				{ mStrBonus = bonus; }
	void SetIntBonus(int bonus)				{ mStrBonus = bonus; }
	void SetAtkBonus(int bonus)				{ mStrBonus = bonus; }
	void SetCritBonus(int bonus)			{ mStrBonus = bonus; }
	void SetHealthBonus(int bonus)			{ mStrBonus = bonus; }
	void SetManaBonus(int bonus)			{ mStrBonus = bonus; }

	void SetDefence(int defence)			{ mDefence = defence; }

	void SetLevelReq(int req)				{ mLevelReq = req; }
	void SetStrReq(int req)					{ mStrReq = req; }
	void SetDefReq(int req)					{ mDefReq = req; }
	void SetDexReq(int req)					{ mDexReq = req; }
	void SetIntReq(int req)					{ mIntReq = req; }
	void SetAtkReq(int req)					{ mAtkReq = req; }

	void UseItem(PlayerInfo& playerInfo, Raknet& raknet);

private:
	int mStrBonus;
	int mDefBonus;
	int mDexBonus;
	int mIntBonus;
	int mAtkBonus;
	int mCritBonus;
	int mHealthBonus;
	int mManaBonus;

	int mDefence;

	int mLevelReq;
	int mStrReq;
	int mDefReq;
	int mDexReq;
	int mIntReq;
	int mAtkReq;
};

#endif