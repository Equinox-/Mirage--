#ifndef _EQIUP_H
#define _EQIUP_H

#include "DragSprite.h"
#include "Raknet.h"
#include "PlayerInfo.h"
#include "ItemBase.h"
#include "Inventory.h"
#include "Stats.h"

class Equip {
public:
	Equip(Raknet& raknet, PlayerInfo& playerInfo, Inventory& inventory);
	~Equip();

	void Init();
	void Update(float deltaTime);
	void UpdateRaknet();
	void Render();

	void ToggleVisible() {
		mDragSprite.ToggleVisible();
	}

private:
	DragSprite mDragSprite;
	Raknet& mRaknet;
	PlayerInfo& mPlayerInfo;
	SGE_Sprite mSprite[8];
	Inventory& mInventory;
	Stats mStats;
	SGE_Font mFont;
};

#endif
