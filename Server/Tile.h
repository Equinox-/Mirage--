#ifndef INCLUDED_TILE_H
#define INCLUDED_TILE_H

#include "SVector2.h"

//====================================================================================================
// Includes
//====================================================================================================

//====================================================================================================
// Class Declarations
//====================================================================================================

class Tile
{
public:
    Tile();
	~Tile();

	// Mutators
	void SetPosition(const SVector2& pos)		{ mPosition = pos; }
	SVector2 GetPosition()							{ return mPosition; }
	void SetType(int id)						{ mID = id; }

private:
	SVector2 mPosition;
	int mID;
};

#endif
