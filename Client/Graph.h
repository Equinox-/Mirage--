#ifndef INCLUDED_AIGRAPH_H
#define INCLUDED_AIGRAPH_H

#include "Node.h"
#include "GroundTile.h"

class IWalkable
{
public:
	//virtual ~IWalkable();
	virtual bool operator()(Node* pNode, int width, int height) = 0;
};

class ICost
{
public:
	//virtual ~ICost();
	virtual float operator() (Node* pNodeA, Node* pNodeB) = 0;
};

class Map;
class Graph
{
public:
	Graph();
	~Graph();

	void Allocate(int width, int height, GroundTile* groundTiles);
	void Purge();

	void SearchAStar(int start, int end, IWalkable& IsWalkable, ICost& GetG, ICost& GetH);
	void SearchDijkstra(int start, int end, IWalkable& IsWalkable, ICost& GetG);

	Node* GetNode(int index) const;
	int GetNumberNodes() const							{ return mNumNodes; }
	const NodeList& GetClosedList() const				{ return mClosedList; }
	const NodeList& GetPath() const						{ return mPath; }

private:
	// No Copy Assign
	Graph(const Graph&);
	Graph& operator = (const Graph&);

	// Helper
	void InsertSortedByG(Node* pNode);
	void InsertSortedByF(Node* pNode);

	NodeList mOpenList;
	NodeList mClosedList;
	NodeList mPath;

	Node* mNodes;

	int mNumNodes;
	int mStart;
	int mEnd;

	int mWidth;
	int mHeight;
};

#endif // INCLUDED_AIGRAPH_H
