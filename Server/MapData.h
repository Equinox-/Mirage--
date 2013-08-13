#ifndef _MAPDATA_H
#define _MAPDATA_H

#include "Tile.h"
#include <stdio.h>
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" 
#include "Defines.h"
#include "GroundTile.h"
#include "NPCData.h"
#include <vector>
#include <list>

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

using namespace RakNet;
using namespace std;

class MapData
{
public:
	MapData();
	~MapData();

	void SetWidth( int width)									{ mWidth = width; }
	void SetHeight( int height)									{ mHeight = height; }
	void CreateTiles();
	void CreateNPCS();
	void SetTileInfo(float x, float y, int tile, int layer, int id);
	void SetMapNumber(int map)									{ mMapNumber = map; }
	void AddItem(int imageNumber, int itemNumber, int index);
	void PlayerDisconnect(RakNetGUID& guid, RakPeerInterface* peer);
	
	void SetWeather(char* weather)								{ memcpy(mWeather, weather, CHAR_MAX); }
	void SetIntensity(int intensity)							{ mWeatherIntensity = intensity; }
	void AddPlayer(RakNetGUID& guid);
	void RemovePlayer(RakNetGUID& guid);
	void SetNPCPath(int arrayIndex, int pathIndex)				{ mNPCS[arrayIndex]->SetPathIndex(pathIndex); }
	void SetNPCPosition(int arrayIndex, SVector2 position)		{ mNPCS[arrayIndex]->SetPosition(position); }
	void SetNPCEndIndex(int arrayIndex, int endIndex)			{ mNPCS[arrayIndex]->mPathEnd = endIndex; }
	void SetNPCStop(int arrayIndex, bool stop)					{ mNPCS[arrayIndex]->SetStop(stop); }
	void SetNorth(int north)									{ mNorth = north; }
	void SetEast(int east)										{ mEast = east; }
	void SetSouth(int south)									{ mSouth = south; }
	void SetWest(int west)										{ mWest = west; }

	int GetWidth()												{ return mWidth; }
	int GetHeight()												{ return mHeight; }
	int GetTileAmount()											{ return mTileAmount; }
	int GetMapNumber()											{ return mMapNumber; }

	Tile* GetLayer(int layer);
	GroundTile* FindItem(int id); 
	RakNetGUID& GetNPCHost()									{ return mHostGUID; }
	

	void SendInfo(RakNet::BitStream& bsOut, Packet* packet);

public:
	GroundTile* mLayer1;
	Tile* mLayer2;
	Tile* mLayer3;
	Tile* mLayer4;
	std::vector<NPCData*> mNPCS;
	std::list<RakNetGUID> mPlayers;


private:
	int mWidth;
	int mHeight;
	int mTileAmount;
	int mMapNumber;
	char mWeather[CHAR_MAX];
	int mWeatherIntensity;
	int mNumberOfNPCS;
	RakNetGUID mHostGUID;

	int mNorth;
	int mEast;
	int mSouth;
	int mWest;
};

#endif
