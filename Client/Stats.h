#ifndef _STATS_H
#define _STATS_H

#include "DragSprite.h"
#include "PlayerInfo.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" 
#include "Raknet.h"

using namespace RakNet;

class Stats
{
public:
	Stats();
	~Stats();

	void Init();
	void Update(float deltaTime);
	void Render();

	void SetPosition(SVector2 position)							{ mPosition = position; }
	void ToggleVisible()										{ mVisible = mVisible == true ? false : true; }

	bool GetVisible()											{ return mVisible; }

private:
	SGE_Sprite mSprite;
	SVector2 mPosition;
	bool mVisible;
};


#endif