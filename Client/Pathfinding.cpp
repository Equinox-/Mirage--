#include "Pathfinding.h"

PathFinding::PathFinding()
:mMapWidth(0)
,mMapHeight(0)
{

}

PathFinding::~PathFinding()
{

}

void PathFinding::Init(int width, int height, GroundTile* groundTiles)
{
	mMapWidth = width;
	mMapHeight = height;

	int tileSize = 32;
	int halfSize = tileSize >> 1;

	mGraph.Allocate(mMapWidth, mMapHeight, groundTiles);

	// Connect nodes 
	for(int y = 0; y < mMapHeight; ++y)
	{
		for(int x = 0; x < mMapWidth; ++x)
		{
			const int index = x + (y * mMapWidth);
			Node* pNode = mGraph.GetNode(index);
				pNode->SetNeighbor(0, mGraph.GetNode((x - 1) + ((y - 1) * mMapWidth)));
			pNode->SetNeighbor(1, mGraph.GetNode((x    ) + ((y - 1) * mMapWidth)));
				pNode->SetNeighbor(2, mGraph.GetNode((x + 1) + ((y - 1) * mMapWidth)));
			pNode->SetNeighbor(3, mGraph.GetNode((x - 1) + ((y    ) * mMapWidth)));
			pNode->SetNeighbor(4, mGraph.GetNode((x + 1) + ((y    ) * mMapWidth)));
				pNode->SetNeighbor(5, mGraph.GetNode((x - 1) + ((y + 1) * mMapWidth)));
			pNode->SetNeighbor(6, mGraph.GetNode((x    ) + ((y + 1) * mMapWidth)));
				pNode->SetNeighbor(7, mGraph.GetNode((x + 1) + ((y + 1) * mMapWidth)));

			pNode->SetId(index);
			pNode->SetPosition(SVector2((float)halfSize + (x * tileSize), (float)halfSize + (y * tileSize)));
		}
	}

	WalkableFunctor walkF = WalkableFunctor();
	ICostFunctor costF = ICostFunctor();
	IHeuristicFunctor heuristicF = IHeuristicFunctor();
	mGraph.SearchAStar(949, 1642, walkF, costF, heuristicF);
}

void PathFinding::Update(float deltaTime)
{
	
}

void PathFinding::Render(SVector2 offset)
{


	/*
	for(int y = 1; y < mMapHeight - 1; ++y)
	{
		for(int x = 1; x <  mMapWidth - 1; ++x)
		{
			const int index = y + (x * mMapWidth);
			Node* pNode = mGraph.GetNode(index);
			for(int n = 0; n < Node::kMaxNeighbors; ++n)
			{
				Node* pNeighbor = pNode->GetNeighbor(n);
				if(pNeighbor)
				{
					Graphics_DebugLine(pNode->GetPosition(), pNeighbor->GetPosition(), 0x00FFFF);
				}
			}
		}
	}
	*/
#ifdef _DEBUG
	if(Input_IsKeyDown(Keys::LCONTROL))
	{
		const NodeList& closedList = mGraph.GetClosedList();
		for(NodeList::const_iterator iter = closedList.begin(); iter != closedList.end(); ++iter)
		{
			Node* pNode = (*iter);
			if(pNode != NULL && pNode->GetParent() != NULL)
			{
				SVector2 pos = pNode->GetPosition();
				SVector2 newPos1 = (pos + offset);

				pos = pNode->GetParent()->GetPosition();
				SVector2 newPos2 = (pos + offset);

				Graphics_DebugLine(newPos1, newPos2, 0x0000FF);
			}
		}

		const NodeList& path = mGraph.GetPath();
		for(NodeList::const_iterator iter = path.begin(); iter != path.end(); ++iter)
		{
			Node* pNode = (*iter);
			if(pNode != NULL && pNode->GetParent() != NULL)
			{
				SVector2 pos = pNode->GetPosition();
				SVector2 newPos1 = (pos + offset);

				pos = pNode->GetParent()->GetPosition();
				SVector2 newPos2 = (pos + offset);

				Graphics_DebugLine(newPos1, newPos2, 0xFF0000);
			}
		}
	}
#endif
}

void PathFinding::Terminate()
{
	mGraph.Purge();
}
