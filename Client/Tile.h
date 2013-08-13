#ifndef INCLUDED_TILE_H
#define INCLUDED_TILE_H

#include <SGE.h>
using namespace SGE;

//====================================================================================================
// Includes
//====================================================================================================

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class Tile
{
public:
    Tile();
    ~Tile();

    // Mutators
    void SetPosition(const SVector2& pos)    { mPosition = pos; }
    void SetType(int type)                   { mType = type; }

	const SVector2& GetPosition()			{ return mPosition; }
	int GetType()							{ return mType; }


	SRect GetBoundingBox() const;


protected:
    SVector2 mPosition;
    int mType;
};

#endif

