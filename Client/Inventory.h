#ifndef _INVENTORY_H
#define _INVENTORY_H

#include "ItemBase.h"
#include "Potion.h"
#include "Weapon.h"
#include "Shield.h"
#include <vector>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" 
#include "Raknet.h"
#include "Map.h"
#include "DragSprite.h"
#include "PlayerInfo.h"
#include "HelperFunctions.h"
#include "Character.h"
#include "Helmet.h"
#include "Armor.h"
#include "Boots.h"
#include "Necklace.h"
#include "Ring.h"

class Inventory
{
public:
	Inventory(int inventorySlots, Raknet& raknet, Map& map, PlayerInfo& playerInfo, Character& character);
	~Inventory();

	void Init();
	void AddItem(ItemBase* item);
	void Update(float deltaTime);
	void UpdateRaknet();
	void Render();
	void Unload();
	void ToggleVisible()							{ mDragSprite.ToggleVisible(); }
	
	const int GetSize()	const						{ return mItems.size(); }
	const int GetSpaceLeft() const					{ return mInventorySlots - mItems.size(); }

private:
	int mInventorySlots;
	std::vector<ItemBase*> mItems;
	Raknet& mRaknet;
	Map& mMap;
	DragSprite mDragSprite;
	PlayerInfo& mPlayerInfo;
	Character& mCharacter;
	SGE_Font mFont;
};

#endif