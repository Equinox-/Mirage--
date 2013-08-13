#include "MapData.h"

MapData::MapData()
:mLayer1(NULL)
,mLayer2(NULL)
,mLayer3(NULL)
,mLayer4(NULL)
,mWidth(0)
,mHeight(0)
,mTileAmount(0)
,mWeatherIntensity(0)
,mNumberOfNPCS(0)
,mNorth(0)
,mEast(0)
,mSouth(0)
,mWest(0)
{

}

MapData::~MapData()
{
	if(mLayer1) { delete[] mLayer1; }
	if(mLayer2) { delete[] mLayer2; }
	if(mLayer3) { delete[] mLayer3; }
	if(mLayer4) { delete[] mLayer4; }	
}


Tile*& MapData::GetLayer(int layer)
{
switch(layer)
	{
	case 1:
		{
			return (Tile*&)mLayer1;
		}
	case 2:
		{
			return mLayer2;
		}
	case 3:
		{
			return mLayer3;
		}
	case 4:
		{
			return mLayer4;
		}
	default:
		{

			break;
		}
	}
}

void MapData::CreateTiles()
{
	if(mTileAmount > 0)
	{
		mLayer1 = new GroundTile[mTileAmount];
		mLayer2 = new Tile[mTileAmount];
		mLayer3 = new Tile[mTileAmount];
		mLayer4 = new Tile[mTileAmount];
	}
}


