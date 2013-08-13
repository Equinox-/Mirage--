#ifndef _NPC_H
#define _NPC_H

#include <SGE.h>
#include "Pathfinding.h"
#include "HelperFunctions.h"
#include "MapData.h"
#include "Direction.h"
#include "Animation.h"
#include "NPCData.h"
#include "Direction.h"
#include "Raknet.h"
#include "Map.h"
using namespace SGE;

class Character;
class Map;
class NPC
{
public:
	NPC(Raknet& raknet, int index);
	~NPC();

	void Init(int mapWidth, int mapHeight, Graph* graph);
	void Update(float deltaTime, Map* map, Character& character);
	void CreatePath();
	void Render(SVector2 offset);
	void Unload();

	void SetEndPath(int index)										{ mNPCData.mPathEnd = index; } 
	void SetStop(bool stop)											{ mNPCData.mStop = stop; }
	
	void ReloadGraphic();
	SRect GetBoundingBox() const;

	NPCData& GetData()												{ return mNPCData; }
	int GetArrayIndex()												{ return mIndex; }
	bool GetStop()													{ return mNPCData.mStop; }
	SVector2 GetPosition()											{ return mNPCData.mPosition; }

private:
	SGE_Sprite mSprite;
	SGE_Font mFont;
	std::vector<SVector2> mPath;
	void CheckCollision(float fSeconds, Map& map, Character& character);

	int mMapWidth;
	int mMapHeight;
	int mIndex;
	bool mReachedDestination;
	bool mBattling;
	bool mInit;

	Animation mAnimation;
	WalkFrames mFrames;
	NPCData mNPCData;
	Direction mDirection;
	Raknet& mRaknet;
	Graph* mGraph;
	SVector2 mVelocity;
};

#endif
