#ifndef _MINIMAP_H
#define _MINIMAP_H

#include <SGE.h>
using namespace SGE;

#include "OtherPlayers.h"
#include "Map.h"

class MiniMap
{
public:
	MiniMap(OtherPlayers& otherPlayers, Map& map, Character& character);
	~MiniMap();

	void Init();
	void Update(float deltaTime);
	void Render();

private:
	SGE_Sprite mMiniMap;
	SGE_Sprite mMiniMapChar;
	SGE_Sprite mMiniMapPlayer;
	SGE_Sprite mMiniMapEnemy;

	OtherPlayers& mOtherPlayers;
	Character& mCharacter;
	Map& mMap;
};

#endif