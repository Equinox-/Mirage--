#ifndef _MAPDATA_H
#define _MAPDATA_H

#include "Tile.h"
#include "GroundTile.h"
#include <stdio.h>
#include "NPCData.h"

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class MapData
{
public:
	MapData();
	~MapData();

	void CreateTiles();
	void SetWidth( int width)										{ mWidth = width; }
	void SetHeight( int height)										{ mHeight = height; }
	void SetNumberOfTiles(int tiles)								{ mTileAmount = tiles; }

	Tile*& GetLayer(int later);
	int GetTileAmount()												{ return mTileAmount; }
	int GetWidth()													{ return mWidth; }
	int GetHeight()													{ return mHeight; }

	bool GetWalkable(int index)										{ return mLayer1[index].IsWalkable(); }

	void Save();
	void Load();

	void SetNorth(int north)									{ mNorth = north; }
	void SetEast(int east)										{ mEast = east; }
	void SetSouth(int south)									{ mSouth = south; }
	void SetWest(int west)										{ mWest = west; }

public:
	GroundTile* mLayer1;
	Tile* mLayer2;
	Tile* mLayer3;
	Tile* mLayer4;
	NPCData* mNPCData;

private:
	int mWidth;
	int mHeight;
	int mTileAmount;
	char mWeather[CHAR_MAX];
	int mWeatherIntensity;
	int mNumberOfNPCS;

	int mNorth;
	int mEast;
	int mSouth;
	int mWest;
};

#endif