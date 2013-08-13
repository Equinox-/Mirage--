#ifndef _ITEM_H
#define _ITEM_H

#include "GroundTile.h"
#include <SGE.h>
using namespace SGE;

struct Item
{
	Item():mTile(NULL){}

	void SetTile(GroundTile* tile)								{ mTile = tile; }

	SGE_Sprite mSprite;
	GroundTile* mTile;
};

#endif