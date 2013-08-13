#include "Node.h"
#include <string.h>

Node::Node()
:mpParent(NULL)
,mId(kInvalidID)
,mPosition(0.0f, 0.0f)
,mOpen(false)
,mClosed(false)
,mWalkable(true)
,mG(0.0f)
,mH(0.0f)
,mCost(1)
{
	memset(mpNeighbors, 0, sizeof(Node*) * kMaxNeighbors);
}

void Node::Reset()
{
	mpParent = NULL;
	mOpen = false;
	mClosed = false;
	mG = 0.0f;
	mH = 0.0f;
}
