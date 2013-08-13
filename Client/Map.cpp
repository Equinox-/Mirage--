#include "Map.h"
#include "Character.h"
#include "NPC.h"
//#include "SLineSegment.h"

Map::Map(Raknet& raknet, PlayerInfo& playerInfo, Character& character) :
		mRaknet(raknet), mPlayerInfo(playerInfo), mMemoryCreated(false), mLoadSprite(
				NULL), mLoadItem(NULL), mTilesetWidth(0), mTilesetHeight(0), mNumberOfItems(
				0), mCharacter(character), mPathFinding(), mNumberOfNPCS(0) {

}

Map::~Map() {
	delete[] mLoadSprite;
	delete[] mSprites;
	delete[] mItemSprites;

	mPathFinding.Terminate();

	for (int a = 0; a < mNumberOfNPCS; ++a) {
		if (mNPCS[a]) {
			delete mNPCS[a];
		}
	}
}

void Map::Init() {
	char temp[CHAR_MAX];
	sprintf(temp, "Maps/Maps%d.oel", mPlayerInfo.GetMap());

	FILE* pFile = fopen(temp, "rb");

	mTilesetWidth = IniFile_GetInt("TILESET_WIDTH", 5);
	mTilesetHeight = IniFile_GetInt("TILESET_HEIGHT", 5);

	// We already have the map
	if (pFile) {
		Load();
	}
	//else

	// We dont have the map better get it
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID) ID_MAP_DATA);
	mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
			mRaknet.mServerAddress, false);

	mWeather.Init();
}

// Load only the tiles we are using in the current map. But do not unload them. Every map we visit will load the new tiles.
// Eventually all tiles will be loaded
void Map::LoadTiles() {
	int width = (mMapData.GetWidth() / 32);
	int height = (mMapData.GetHeight() / 32);

	// Create the sprites
	int numberOfSprites = (mTilesetWidth * mTilesetHeight) + 1;
	int numMapTiles = mMapData.GetTileAmount();

	if (!mMemoryCreated) {
		mSprites = new SGE_Sprite[numberOfSprites];
		mLoadSprite = new bool[numberOfSprites];
		mMemoryCreated = true;
	}

	// Clear the array
	for (int a = 0; a < numberOfSprites; ++a) {
		mLoadSprite[a] = false;
	}

	for (int a = 0; a < numMapTiles; ++a) {
		// If we have a sprite of this type we want to load it. Otherwise we dont
		mLoadSprite[mMapData.mLayer1[a].GetType()] = true;
		mLoadSprite[mMapData.mLayer2[a].GetType()] = true;
		mLoadSprite[mMapData.mLayer3[a].GetType()] = true;
		mLoadSprite[mMapData.mLayer4[a].GetType()] = true;
	}

	// Load all the tiles
	int count = 0;
	for (int a = 0; a < mTilesetHeight; ++a) {
		for (int b = 0; b < mTilesetWidth; ++b) {
			char temp[CHAR_MAX];
			sprintf(temp, "Tiles/Tile_%d_%d.png", a + 1, b + 1);

			if (mLoadSprite[count]) {
				mSprites[count].Load(temp);
				mLoadSprite[count] = false;
			}

			++count;
		}
	}
}

void Map::LoadItems() {
	mItemSprites = new Item[mNumberOfItems];
	mLoadItem = new bool[mNumberOfItems];

	// Clear array
	for (int a = 0; a < mNumberOfItems; ++a) {
		mLoadItem[a] = false;
	}

	int numMapTiles = mMapData.GetTileAmount();

	for (int a = 0; a < numMapTiles; ++a) {
		int item = mMapData.mLayer1[a].GetItemImageNumber();
		if (item > 0 && item < mNumberOfItems && mLoadItem[item] == false) {
			char temp[CHAR_MAX];
			sprintf(temp, "Items/Item_%d.png", item);
			mItemSprites[item].mSprite.Load(temp);
			mItemSprites[item].mTile = &mMapData.mLayer1[a];
			mLoadItem[item] = true;
		}
	}
}

void Map::Update(float deltaTime) {
	mWeather.Update(deltaTime);
	mPathFinding.Update(deltaTime);

	for (int a = 0; a < mNumberOfNPCS; ++a) {
		mNPCS[a]->Update(deltaTime, this, mCharacter);
	}
}

static SVector2 sOffset;
void Map::Render(const SVector2& offset, OtherPlayers& otherPlayers) {
	// Used in bounding box checks later
	sOffset = offset;

	int tiles = mMapData.GetTileAmount();

	// Render Ground
	for (int a = 0; a < tiles; ++a) {
		GroundTile* pTile = &mMapData.mLayer1[a];
		int type = pTile->GetType();
		if (type) {
			const SVector2& pos = pTile->GetPosition();
			mSprites[type].SetPosition(pos + offset);
			mSprites[type].Render();
		}
	}

	// Render Mask
	for (int a = 0; a < tiles; ++a) {
		Tile* pTile = &mMapData.mLayer2[a];
		int type = pTile->GetType();
		if (type) {
			const SVector2& pos = pTile->GetPosition();
			mSprites[type].SetPosition(pos + offset);
			mSprites[type].Render();
		}

		// Render Items
		if (mMapData.mLayer1[a].GetItemImageNumber() > 0 && GetItemActive(a)) {
			SVector2 pos = SVector2(mMapData.mLayer1[a].GetPosition().y,
					mMapData.mLayer1[a].GetPosition().x); // Bug
			int type = mMapData.mLayer1[a].GetItemImageNumber();
			if (type > 0 && type < mNumberOfItems) {
				mItemSprites[type].mSprite.SetPosition(pos + offset);
				mItemSprites[type].mSprite.Render();
			}
		}
	}

	// Render NPCS
	for (int a = 0; a < mNumberOfNPCS; ++a) {
		mNPCS[a]->Render(offset);
	}

	// Render Character
	mCharacter.Render(offset);

	// Render other players
	otherPlayers.Render(offset);

	// Render Extra
	for (int a = 0; a < tiles; ++a) {
		SVector2 pos = mMapData.mLayer3[a].GetPosition();
		int type = mMapData.mLayer3[a].GetType();
		mSprites[type].SetPosition(pos + offset);
		mSprites[type].Render();
	}

	// Render Fringe
	for (int a = 0; a < tiles; ++a) {
		SVector2 pos = mMapData.mLayer4[a].GetPosition();
		int type = mMapData.mLayer4[a].GetType();
		mSprites[type].SetPosition(pos + offset);
		mSprites[type].Render();
	}

	mWeather.Render(offset);
	mPathFinding.Render(offset);
}

void Map::Unload() {

}

void Map::UpdateRaknet() {
	switch (mRaknet.mPacket->data[0]) {
	case ID_ITEM_TAKEN: {
		int itemIndex = 0;

		RakNet::BitStream bsIn(mRaknet.mPacket->data, mRaknet.mPacket->length,
				false);
		bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		bsIn.Read(itemIndex);

		mMapData.mLayer1[itemIndex].SetActive(false);
		break;
	}
	case ID_DROP_ITEM: {
		int imageNumber = 0;
		int index = 0;

		RakNet::BitStream bsIn(mRaknet.mPacket->data, mRaknet.mPacket->length,
				false);
		bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

		bsIn.Read(imageNumber);
		bsIn.Read(index);

		AddItem(imageNumber, index);
		break;
	}
	case ID_MAP_DATA: {
		int map = 0;
		int width = 0;
		int height = 0;
		int tiles = 0;

		RakNet::BitStream bsIn(mRaknet.mPacket->data, mRaknet.mPacket->length,
				false);
		bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		bsIn.Read(map);
		bsIn.Read(width);
		bsIn.Read(height);
		bsIn.Read(tiles);

		mMapData.SetWidth(width);
		mMapData.SetHeight(height);
		mMapData.SetNumberOfTiles(tiles);
		mMapData.CreateTiles();

		for (int a = 0; a < tiles; ++a) {
			bsIn.Read(mMapData.mLayer1[a]);
			bsIn.Read(mMapData.mLayer2[a]);
			bsIn.Read(mMapData.mLayer3[a]);
			bsIn.Read(mMapData.mLayer4[a]);
		}

		bsIn.Read(mNumberOfNPCS);

		for (int a = 0; a < mNumberOfNPCS; ++a) {
			NPC* temp = new NPC(mRaknet, a);
			bsIn.Read(temp->GetData());
			mNPCS.push_back(temp);
		}

		// Save map so we dont need to retrieve it every time
		Save();

		// Create are tiles for the map
		mNumberOfItems = IniFile_GetInt("Number_Of_items", 5);

		LoadTiles();
		LoadItems();
		mPathFinding.Init(GetWidth(), GetHeight(), &mMapData.mLayer1[0]);

		// Load npcs
		for (int a = 0; a < mNumberOfNPCS; ++a) {
			NPC* temp = mNPCS[a];
			NPCData& tempData = temp->GetData();

			temp->Init(width, height, mPathFinding.GetGraph());
			temp->ReloadGraphic();

			if (!tempData.mHost) {
				temp->CreatePath();
			}
		}

		int north = 0;
		int east = 0;
		int south = 0;
		int west = 0;

		bsIn.Read(north);
		bsIn.Read(east);
		bsIn.Read(south);
		bsIn.Read(west);

		mMapData.SetNorth(north);
		mMapData.SetEast(east);
		mMapData.SetSouth(south);
		mMapData.SetWest(west);

		break;
	}

	case ID_NPC_NEW_PATH: {
		int npcSlot = 0;
		int endIndex = 0;
		SVector2 position(0.0f, 0.0f);

		RakNet::BitStream bsIn(mRaknet.mPacket->data, mRaknet.mPacket->length,
				false);
		bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		bsIn.Read(npcSlot);
		bsIn.Read(endIndex);
		bsIn.Read(position);

		NPC* tempNPC = mNPCS[npcSlot];
		tempNPC->SetEndPath(endIndex);
		tempNPC->CreatePath();
		tempNPC->GetData().SetPosition(position);
		break;
	}

	case ID_NPC_HOST: {
		for (int a = 0; a < mNumberOfNPCS; ++a) {
			mNPCS[a]->GetData().SetHost(true);
		}
		break;
	}

	case ID_NPC_STOP: {
		if (!mMemoryCreated)
			break;

		int npcSlot = 0;
		bool stop = false;

		RakNet::BitStream bsIn(mRaknet.mPacket->data, mRaknet.mPacket->length,
				false);
		bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		bsIn.Read(npcSlot);
		bsIn.Read(stop);

		if (stop) {
			int test = 0;
		}
		mNPCS[npcSlot]->SetStop(stop);
		break;
	}
	}
}

void Map::Save() {

}

void Map::Load() {

}

SRect Map::GetBoundingBoxFromSegment(SLineSegment line, Character& character) {
	// Validate that the line segment is within the maps range
	if (line.from.x < 0.0f || line.from.x > mMapData.GetWidth()
			|| line.from.y < 0.0f || line.from.y > mMapData.GetHeight()
			|| line.to.x < 0.0f || line.to.x > mMapData.GetWidth()
			|| line.to.y < 0.0f || line.to.y > mMapData.GetHeight()) {
		return SRect();
	}

	// Convert position to indices
	const int fromX = static_cast<int>(line.from.x) / 32;
	const int fromY = static_cast<int>(line.from.y) / 32;
	const int toX = static_cast<int>(line.to.x) / 32;
	const int toY = static_cast<int>(line.to.y) / 32;

	// Calculate Tile Count
	const int countX = toX - fromX + 1;
	const int countY = toY - fromY + 1;

	// Get Region
	SRect region;

#if _DEBUG

	for(int a = 0; a < mNumberOfNPCS; ++a)
	{
		SRect region2 = mNPCS[a]->GetBoundingBox();
		Graphics_DebugRect(region2 + sOffset, 0XFF00FF);

		Graphics_DebugRect(character.GetBoundingBox() + sOffset, 0X00FFFF);
	}

	for(int a = 0; a < mMapData.GetTileAmount(); ++a)
	{
		if(!mMapData.mLayer1[a].IsWalkable())
		{
			SRect region2 = mMapData.mLayer1[a].GetBoundingBox();
			Graphics_DebugRect(region2 + sOffset, 0XFF00FF);
		}
	}

#endif

	int width = GetWidth();
	for (int y = 0; y < countY; ++y) {
		for (int x = 0; x < countX; ++x) {
			const int index = (fromX + x) + ((fromY + y) * (width));

			character.SetTileIndex(index);

			// Block Tile
			if (!mMapData.mLayer1[index].IsWalkable()) {
				region += mMapData.mLayer1[index].GetBoundingBox();
				Graphics_DebugRect(region + sOffset, 0XFF0000);
			}

			for (int a = 0; a < mNumberOfNPCS; ++a) {
				bool npcIntersect = PointInRect(line.to,
						mNPCS[a]->GetBoundingBox());

				if (npcIntersect) {
					return region += mNPCS[a]->GetBoundingBox();
				}
			}

			// Item Tile
			if (mMapData.mLayer1[index].GetItemImageNumber()) {
				SRect debug = region + mMapData.mLayer1[index].GetBoundingBox();
				character.SetOnItem(mMapData.mLayer1[index].GetItemID());
				character.SetOnItemIndex(index);
				Graphics_DebugRect(debug + sOffset, 0XFFFF00);
			} else {
				character.SetOnItem(0);
			}
		}
	}

	return region;
}

SRect Map::GetBoundingBoxFromSegment(SLineSegment line, NPC& npc,
		Character& character) {
	// Get Region
	SRect region;

	//bool npcIntersect = PointInRect(line.to, character.GetBoundingBox()) || PointInRect(line.from, character.GetBoundingBox()) ;
	bool npcIntersect = Intersect(line, character.GetBoundingBox());
	//npc.SetStop(npcIntersect);

	if (npcIntersect) {
		region += character.GetBoundingBox();
	}

	return region;
}

void Map::AddItem(int itemImageNumber, int index) {
	mMapData.mLayer1[index].AddItem(index, itemImageNumber);

	SetItemActive(index, true);

	int item = mMapData.mLayer1[index].GetItemImageNumber();
	GroundTile* tile = &mMapData.mLayer1[index];

	if (item != 0 && mLoadItem[item] == false) {
		char temp[CHAR_MAX];
		sprintf(temp, "Items/Item_%d.png", item);
		mItemSprites[item].mSprite.Load(temp);
		mItemSprites[item].mTile = tile;
		mLoadItem[item] = true;
	}

	tile->SetItemImageNumber(itemImageNumber);
}
