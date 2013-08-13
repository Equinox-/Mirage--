#ifndef _GROUNDTILE_H
#define _GROUNDTILE_H

#include "Tile.h"

class GroundTile: public Tile {
public:
	GroundTile() :
			mWalkable(true), mItemImage(0), mItemID(0), mItemNumber(0), mIndex(
					0), mActive(true), mNPC(-1) {
	}
	~GroundTile() {
	}

	// Mutators
	void SetWalkable(bool walkable) {
		mWalkable = walkable;
	}
	void SetItemImageNumber(int item) {
		mItemImage = item;
	}
	void SetActive(bool active) {
		mActive = active;
	}
	void SetIndex(int index) {
		mIndex = index;
	}
	void AddItem(int itemID, int itemImageNumber) {
		mItemID = itemID;
		mItemImage = itemImageNumber;
	}
	void SetNPC(int npcIndex) {
		mNPC = npcIndex;
	}

	bool IsWalkable() {
		return mWalkable;
	}
	bool IsActive() {
		return mActive;
	}
	int GetItemImageNumber() {
		return mItemImage;
	}
	int GetItemID() {
		return mItemID;
	}
	int GetIndex() {
		return mIndex;
	}
	int GetItemNumber() {
		return mItemNumber;
	}
	int GetNPC() {
		return mNPC;
	}

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
