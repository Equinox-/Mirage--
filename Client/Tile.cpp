#include "Tile.h"

Tile::Tile()
: mPosition(0.0f, 0.0f)
, mType(0)
{

}

Tile::~Tile(void)
{

}

SRect Tile::GetBoundingBox() const
{
	return SRect
	(
		mPosition.y,
		mPosition.x, 
		mPosition.y + 32,
		mPosition.x + 32
	);
}
