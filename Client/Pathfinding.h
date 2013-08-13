#ifndef _PATHFINDING_H
#define _PATHFINDING_H

#include "Graph.h"
#include <SGE.h>
#include <math.h>
#include <stdlib.h>
using namespace SGE;

class ICostFunctor: public ICost {
public:
	virtual float operator()(Node* pNodeA, Node* pNodeB) {
		// Manhattan distance for "Heuristic"
		//float xCost = abs(pNodeA->GetPosition().x - pNodeB->GetPosition().x);
		//float yCost = abs(pNodeA->GetPosition().y - pNodeB->GetPosition().y);
		//return (xCost + yCost);
		return Distance(pNodeA->GetPosition(), pNodeB->GetPosition());
	}
};

class IHeuristicFunctor: public ICost {
public:
	virtual float operator()(Node* pNodeA, Node* pNodeB) {
		float xCost = abs(pNodeA->GetPosition().x - pNodeB->GetPosition().x);
		float yCost = abs(pNodeA->GetPosition().y - pNodeB->GetPosition().y);
		return (xCost + yCost);
	}
};

class WalkableFunctor: public IWalkable {
public:
	// Implements IWalkable
	virtual bool operator()(Node* pNode, int width, int height) {
		bool walkable = true;
		int x = pNode->GetId() / height;
		int y = pNode->GetId() % height;

		if (x == 0 || x == width - 1 || y == 0 || y == height - 1
				|| !pNode->IsWalkable()) {
			walkable = false;
		}
		return walkable;
	}
};

class PathFinding {
public:
	PathFinding();
	~PathFinding();

	void Init(int width, int height, GroundTile* groundTiles);
	void Update(float deltaTime);
	void Render(SVector2 offset);
	void Terminate();
	Graph* GetGraph() {
		return &mGraph;
	}

private:
	int mMapWidth;
	int mMapHeight;
	Graph mGraph;
};

#endif
