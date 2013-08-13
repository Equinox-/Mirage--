#ifndef _MAP_LOAD_H
#define _MAP_LOAD_H

#define __MSXML_LIBRARY_DEFINED__ // Fix Stupid XML Document library errors
#include "tinyxml2.h"
#include "MapData.h"
using namespace tinyxml2;
#include "ActiveMaps.h"
#include "Potion.h"
#include "Weapon.h"
#include "Helmet.h"
#include "Armor.h"
#include "Boots.h"
#include "Necklace.h"

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class ActiveMapItems;
class MapLoad
{
public:
	MapLoad(MapData& mapData, ActiveMaps& activeMaps);
	~MapLoad();

	void Load(int mapNumber);
	void ReadLayer(int layer);
	void ReadAttributes(int mapNumber);
	int ReadItemImageNumber(int itemNumber);

private:

	tinyxml2::XMLDocument mDoc;
	XMLElement* mRootElement;
	const XMLAttribute* mAttributeElement;
	MapData& mMapData;
	ActiveMaps& mActiveMaps;
};

#endif