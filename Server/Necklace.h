#ifndef _NECKLACE_H
#define _NECKLACE_H

#include "ItemBase.h"

class Necklace : public ItemBase
{
public:
	Necklace();
	~Necklace();

	virtual void Init();
	void SendData(BitStream& bsOut);

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

	int GetStrBonus()						{ return mStrBonus; }
	int GetDefBonus()						{ return mDefBonus; }
	int GetDexBonus()						{ return mDexBonus; }
	int GetIntBonus()						{ return mIntBonus; }
	int GetAtkBonus()						{ return mAtkBonus; }
	int GetCritBonus()						{ return mCritBonus; }
	int GetHealthBonus()					{ return mHealthBonus; }
	int GetManaBonus()						{ return mManaBonus; }
	int GetDefence()						{ return mDefence; }

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