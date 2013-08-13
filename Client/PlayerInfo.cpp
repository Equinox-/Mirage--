#include "PlayerInfo.h"

PlayerInfo::PlayerInfo()
:mPosition(0.0f, 0.0f)
,mLevel(0)
,mBanned(false)
,mMap(0)
,mSpriteNumber(0)
,mDirection(down)
,mCurrentHealth(100)
,mTotalHealth(100)
,mCurrentMana(100)
,mTotalMana(100)
,mCurrentExp(0)
,mTotalExp(100)
,mStr(5)
,mDex(5)
,mInt(5)
,mAttack(5)
,mDefence(5)
,mCritChance(1)
,mAccessLevel(0)
,mGold(0)
{
	// Clear Items
	for(int a = 0; a < 50; ++a)
	{
		mItems[a].first = 0;
		mItems[a].second = 0;
	}

	for(int a = 0; a < 9; ++a)
	{
		mEqiupment[a].first = 0;
		mEqiupment[a].second = 0;
	}
}

PlayerInfo::~PlayerInfo()
{

}


void PlayerInfo::AddHealth(int health)
{
	mCurrentHealth += health;

	if(mCurrentHealth > mTotalHealth)
	{
		mCurrentHealth = mTotalHealth;
	}
}

void PlayerInfo::AddMana(int mana)
{
	mCurrentMana += mana;

	if(mCurrentMana > mTotalMana)
	{
		mCurrentMana = mTotalMana;
	}
}

void PlayerInfo::SetEqiupItem(int type, int itemNumber, int imageNumber)
{
	if(type != ring)
	{
		mEqiupment[type].first = itemNumber;
		mEqiupment[type].second = imageNumber;
	}
	else
	{
		// If the second ring is full swap out the first one
		if(mEqiupment[ring2].first > 0)
		{
			mEqiupment[ring].first = itemNumber;
			mEqiupment[ring].second = imageNumber;
		}
		else // Second slot is empty fill that one
		{
			mEqiupment[ring2].first = itemNumber;
			mEqiupment[ring2].second = imageNumber;
		}
	}
}

int PlayerInfo::GetEqiupItemImage(int slot)
{
	return mEqiupment[slot].second;
}