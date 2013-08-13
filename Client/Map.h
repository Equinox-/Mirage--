#ifndef _MAP_H
#define _MAP_H

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" 
#include "Raknet.h"
#include "MapData.h"
#include "PlayerInfo.h"
#include "Character.h"
#include "OtherPlayers.h"
#include "Item.h"
#include "Character.h"
#include "Weather.h"
#include "Pathfinding.h"
#include "NPC.h"
#include <vector>

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class Character;
class PlayerInfo;
class OtherPlayers;
class NPC;

class Map {
public:
	Map(Raknet& raknet, PlayerInfo& playerInfo, Character& character);
	~Map();

	void Init();
	void Update(float deltaTime);
	void Unload();
	void Render(const SVector2& offset, OtherPlayers& otherPlayers);
	void LoadTiles();
	void LoadItems();

	SRect GetBoundingBoxFromSegment(SLineSegment line, Character& character);
	SRect GetBoundingBoxFromSegment(SLineSegment line, NPC& npc,
			Character& character);

	void UpdateRaknet();
	void Load();
	void Save();

	int GetWidth() {
		return mMapData.GetWidth() / 32;
	}
	int GetHeight() {
		return mMapData.GetHeight() / 32;
	}
	int CheckItem(int index) {
		return mMapData.mLayer1[index].GetItemImageNumber();
	}
	int GetNumberOfItems() {
		return mNumberOfItems;
	}

	void GenerateIDList();
	void SetItemActive(int index, bool active) {
		mMapData.mLayer1[index].SetActive(active);
	}
	void AddItem(int itemImageNumber, int index);

	Item* GetItemSprite(int number) {
		return &mItemSprites[number];
	}
	bool GetItemActive(int index) {
		return mMapData.mLayer1[index].IsActive();
	}
	bool GetWalkable(int index) {
		return mMapData.mLayer1[index].IsWalkable();
	}
	NPC* GetNPC(int index) {
		if (mNPCS.size() == 0)
			return NULL;
		if (mNPCS[index]) {
			return mNPCS[index];
		}
		return NULL;
	}
	int GetNumberOfNPCS() {
		return mNumberOfNPCS;
	}

private:
	Raknet& mRaknet;
	MapData mMapData;
	PlayerInfo& mPlayerInfo;
	Character& mCharacter;
	SGE_Sprite* mSprites;
	Item* mItemSprites;
	Weather mWeather;
	PathFinding mPathFinding;
	std::vector<NPC*> mNPCS;

	bool* mLoadSprite;
	bool* mLoadItem;
	bool mMemoryCreated;

	int mTilesetWidth;
	int mTilesetHeight;
	int mNumberOfItems;
	int mNumberOfNPCS;

};

#endif
