#ifndef _NPCDATA_H
#define _NPCDATA_H

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "SVector2.h"

enum NPCTypes
{
	friendly
};

struct NPCData
{
	NPCData()
	:mMoveSpeed(1.0f)
	,mDamage(0)
	,mDefence(0)
	,mItemNumber(-1)
	,mSpriteNumber(1)
	,mPathIndex(0)
	,mHost(false)
	,mPosition(0.0f, 0.0f)
	,mPathEnd(0)
	,mCanMove(true)
	,mStop(false)
	,mCurrentHealth(100)
	,mTotalHealth(100)
	{
		memset(mName, '\0', CHAR_MAX);
		memset(mSpeech, '\0', CHAR_MAX * 2);

		sprintf(mName, "None");
	}

	void SetHost(bool host)											{ mHost = host; }
	void SetPosition(SVector2 position)								{ mPosition = position; }
	void SetPathIndex(int index)									{ mPathIndex = index; }
	void SetStop(bool stop)											{ mStop = stop; }

	char mName[CHAR_MAX];
	char mSpeech[CHAR_MAX * 2];
	SVector2 mPosition;

	float mMoveSpeed;
	int mDamage;
	int mDefence;
	int mItemNumber;
	int mSpriteNumber;
	int mPathIndex;
	int mPathEnd;
	bool mHost;
	bool mCanMove;
	bool mStop;

	float mCurrentHealth;
	float mTotalHealth;
};

#endif
