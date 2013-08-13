#include "MapLoad.h"

MapLoad::MapLoad(MapData& mapData, ActiveMaps& activeMaps) :
		mActiveMaps(activeMaps), mMapData(mapData) {

}

MapLoad::~MapLoad() {

}

void MapLoad::Load(int mapNumber) {
	char fileName[CHAR_MAX];
	sprintf(fileName, "Maps/Map%d.oel", mapNumber);
	mDoc.LoadFile(fileName);

	// Set the root element to the map
	mRootElement = mDoc.FirstChildElement("level");
	mAttributeElement = mRootElement->FirstAttribute();

	// Set map Dimensions
	mMapData.SetWidth(mAttributeElement->IntValue());
	++mAttributeElement;
	mMapData.SetHeight(mAttributeElement->IntValue());

	++mAttributeElement;
	++mAttributeElement;
	++mAttributeElement;
	//mMapData.SetWeather((char*)mAttributeElement);

	mMapData.SetNorth(mAttributeElement->IntValue());
	++mAttributeElement;
	mMapData.SetEast(mAttributeElement->IntValue());
	++mAttributeElement;
	mMapData.SetSouth(mAttributeElement->IntValue());
	++mAttributeElement;
	mMapData.SetWest(mAttributeElement->IntValue());

	// Create the actual tiles
	mMapData.CreateTiles();

	// Read the layers
	ReadLayer(1);
	ReadLayer(2);
	ReadLayer(3);
	ReadLayer(4);

	// Read Attributes
	ReadAttributes(mapNumber);

	mMapData.CreateNPCS();
}

void MapLoad::ReadLayer(int layer) {
	switch (layer) {
	case 1: {
		mRootElement = mDoc.FirstChildElement("level")->FirstChildElement(
				"Ground")->FirstChildElement("tile");
		break;
	}

	case 2: {
		mRootElement = mDoc.FirstChildElement("level")->FirstChildElement(
				"Mask")->FirstChildElement("tile");
		break;
	}

	case 3: {
		mRootElement = mDoc.FirstChildElement("level")->FirstChildElement(
				"Fringe")->FirstChildElement("tile");
		break;
	}

	case 4: {
		mRootElement = mDoc.FirstChildElement("level")->FirstChildElement(
				"Extra")->FirstChildElement("tile");
		break;
	}
	}

	// Get the tiles
	int count = 0;
	for (const XMLNode* node = mRootElement; node; node = node->NextSibling()) {
		const tinyxml2::XMLElement *tileElem = node->ToElement();

		// Read the Global-ID of the tile.
		float x = (float) tileElem->IntAttribute("x");
		float y = (float) tileElem->IntAttribute("y");
		int id = tileElem->IntAttribute("id");

		mMapData.SetTileInfo(x * 32, y * 32, count, layer, id);
		++count;
	}
}

void MapLoad::ReadAttributes(int mapNumber) {
	mMapData.SetMapNumber(mapNumber);
	mRootElement = mDoc.FirstChildElement("level")->FirstChildElement(
			"Attributes")->FirstChildElement("Block");

	for (const XMLNode* node = mRootElement; node; node = node->NextSibling()) {
		const tinyxml2::XMLElement *tileElem = node->ToElement();

		// Read the Global-ID of the tile.
		float x = (float) tileElem->IntAttribute("x");
		float y = (float) tileElem->IntAttribute("y");
		int index = (x / 32) + ((y / 32) * (mMapData.GetHeight() / 32));

		mMapData.mLayer1[index].SetWalkable(false);
	}

	// Items
	mRootElement = mDoc.FirstChildElement("level")->FirstChildElement(
			"Attributes")->FirstChildElement("Item");

	for (const XMLNode* node = mRootElement; node; node = node->NextSibling()) {
		const tinyxml2::XMLElement *tileElem = node->ToElement();

		// Read the Global-ID of the tile.
		int id = tileElem->IntAttribute("id");
		float x = (float) tileElem->IntAttribute("x");
		float y = (float) tileElem->IntAttribute("y");
		int index = (x / 32.0f)
				+ ((y / 32.0f) * ((float) mMapData.GetWidth() / 32));

		int itemNumber = tileElem->IntAttribute("ItemNumber");

		mMapData.mLayer1[index].SetItemNumber(itemNumber);
		mMapData.mLayer1[index].SetItemImageNumber(
				ReadItemImageNumber(itemNumber));
		mMapData.mLayer1[index].SetWalkable(true);
		mMapData.mLayer1[index].SetItemID(id);
		mMapData.mLayer1[index].SetIndex(index);
	}

	// NPCS
	mRootElement = mDoc.FirstChildElement("level")->FirstChildElement(
			"Attributes")->FirstChildElement("NPC");

	for (const XMLNode* node = mRootElement; node; node = node->NextSibling()) {
		const tinyxml2::XMLElement *tileElem = node->ToElement();

		float x = (float) tileElem->IntAttribute("x");
		float y = (float) tileElem->IntAttribute("y");
		int index = (x / 32.0f)
				+ ((y / 32.0f) * ((float) mMapData.GetWidth() / 32));
		int npc = tileElem->IntAttribute("NPCNumber");

		mMapData.mLayer1[index].SetNPC(npc);
	}

	mActiveMaps.AddMap(&mMapData);
}

int MapLoad::ReadItemImageNumber(int itemNumber) {
	char temp[CHAR_MAX];
	sprintf(temp, "Items/%d.item", itemNumber);
	FILE* pFile = fopen(temp, "rb");
	int imageNumber = 0;

	int itemType = 0;

	if (pFile) {
		fscanf(pFile, "%d", &itemType);

		switch (itemType) {
		case potion: {
			Potion potion;
			fread(&potion, 1, sizeof(potion), pFile);
			return potion.GetImageNumber();
		}

		case weapon: {
			Weapon weapon;
			fread(&weapon, 1, sizeof(weapon), pFile);
			return weapon.GetImageNumber();
		}
		case shield: {
			Shield shield;
			fread(&shield, 1, sizeof(Shield), pFile);
			return shield.GetImageNumber();
		}
		case helmet: {
			Helmet helmet;
			fread(&helmet, 1, sizeof(Helmet), pFile);
			return helmet.GetImageNumber();
		}
		case armor: {
			Armor armor;
			fread(&armor, 1, sizeof(Armor), pFile);
			return armor.GetImageNumber();
		}
		case boots: {
			Boots boots;
			fread(&boots, 1, sizeof(Boots), pFile);
			return boots.GetImageNumber();
		}
		case necklace: {
			Necklace necklace;
			fread(&necklace, 1, sizeof(Necklace), pFile);
			return necklace.GetImageNumber();
		}
		case ring: {
			Ring ring;
			fread(&ring, 1, sizeof(Ring), pFile);
			return ring.GetImageNumber();
		}
		}
	}

	return 0;
}
