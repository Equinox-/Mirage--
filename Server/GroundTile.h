#ifndef _GROUNDTILE_H
#define _GROUNDTILE_H

#include "Tile.h"

class GroundTile : public Tile
{
public:
	GroundTile():mWalkable(true), mItemImage(0), mActive(true), mItemID(0), mItemNumber(0),mIndex(0), mNPC(-1) {};
	~GroundTile() {};

	// Mutators
	void SetWalkable(bool walkable)				{ mWalkable = walkable; }
	void SetItemImageNumber(int item)			{ mItemImage = item; }
	void SetItemID(int item)					{ mItemID = item; }
	void SetActive(bool active)					{ mActive = active; }
	void SetItemNumber(int number)				{ mItemNumber = number; }
	void SetIndex(int index)					{ mIndex = index; }
	void SetNPC(int number)						{ mNPC = number; }

	int GetItemID()								{ return mItemID; }
	bool GetActive()							{ return mActive; }
	int GetItemImageNumber()					{ return mItemImage; }
	int GetItemNumber()							{ return mItemNumber; }
	int GetIndex()								{ return mIndex; }
	int GetNPC()								{ return mNPC; }

protected:
    bool mWalkable;
	int mItemImage;
	int mItemID;
	int mItemNumber;
	int mIndex;
	bool mActive;
	int mNPC;
};


#endif