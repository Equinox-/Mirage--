#include "Graph.h"

Graph::Graph()
:mNodes(NULL)
,mNumNodes(0)
,mStart(Node::kInvalidID)
,mEnd(Node::kInvalidID)
,mWidth(0)
,mHeight(0)
{

}

Graph::~Graph()
{
	Purge();
}

void Graph::Allocate(int width, int height, GroundTile* groundTiles)
{
	Purge();

	mNumNodes = width * height;

	if(mNumNodes > 0)
	{
		mNodes = new Node[mNumNodes];
	}

	mWidth = width;
	mHeight = height;


	// Check if all the tiles our walkable
	for(int a = 0; a < mNumNodes; ++a)
	{
		if(!groundTiles[a].IsWalkable())
		{
			mNodes[a].SetWalkable(false);
		}
	}
}

void Graph::Purge()
{
	if(mNodes != NULL)
	{
		delete [] mNodes;
		mNodes = NULL;
	}

	mNumNodes = 0;
}

Node* Graph::GetNode(int index) const
{
	Node* node = NULL;
	if(index >= 0 && index < mNumNodes)
	{
		node = &mNodes[index];
	}

	return node;
}

void Graph::SearchDijkstra(int start, int end, IWalkable& IsWalkable, ICost& GetG)
{
	mStart = start;
	mEnd = end;

	 // Reset
	for(int i = 0; i < mNumNodes; ++i)
	{
		mNodes[i].Reset();
	}

	mOpenList.clear();
	mClosedList.clear();

	// Put Start Node in the open list
	mOpenList.push_back(&mNodes[mStart]);
	mNodes[mStart].Open();

	// Loop until we are done
	bool found = false;
	while(!found && !mOpenList.empty())
	{
		Node* pNode = mOpenList.front();
		mOpenList.pop_front();
		
		//Check if this is teh end node
		if(pNode == &mNodes[mEnd])
		{
			found = true;
		}
		else
		{
			for(int n = 0; n < Node::kMaxNeighbors; ++n)
			{
				Node* pNeighbor = pNode->GetNeighbor(n);
				if(pNeighbor != NULL && !pNeighbor->IsClosed() && IsWalkable(pNeighbor, mWidth, mHeight))
				{
					float G = pNode->GetG() + GetG(pNode, pNeighbor);

					if(!pNeighbor->IsOpen() )	
					{
						// Set parent
						pNeighbor->SetParent(pNode);

						// Set Cost
						pNeighbor->SetG(G);

						// Insert into open List
						InsertSortedByG(pNeighbor);
						pNeighbor->Open();
					}
					else if(pNeighbor->GetG() > G)
					{
						// Update Parent
						pNeighbor->SetParent(pNode);
						
						// Update cost
						pNeighbor->SetG(G);

						// Re-Insert to open list
						mOpenList.remove(pNeighbor);
						InsertSortedByG(pNeighbor);

					}
				}
			}
		}

		// Add Node to closed list
		mClosedList.push_front(pNode);
		pNode->Close();
	}

	// Check if we have a path
	if (found)
	{
		// Store path later perhaps
		// Clear path
		mPath.clear();

		// Construct path from end to start
		Node* pNode = &mNodes[mEnd];
		while(pNode != NULL)
		{
			mPath.push_front(pNode);
			pNode = pNode->GetParent();
		}
	}
}


void Graph::SearchAStar(int start, int end, IWalkable& IsWalkable, ICost& GetG, ICost& GetH)
{
	mStart = start;
	mEnd = end;

	 // Reset
	for(int i = 0; i < mNumNodes; ++i)
	{
		mNodes[i].Reset();
	}

	mOpenList.clear();
	mClosedList.clear();

	// Put Start Node in the open list
	mOpenList.push_back(&mNodes[mStart]);
	mNodes[mStart].Open();

	// Loop until we are done
	bool found = false;
	while(!found && !mOpenList.empty())
	{
		Node* pNode = mOpenList.front();
		mOpenList.pop_front();
		
		//Check if this is teh end node
		if(pNode == &mNodes[mEnd])
		{
			found = true;
		}
		else
		{
			for(int n = 0; n < Node::kMaxNeighbors; ++n)
			{
				Node* pNeighbor = pNode->GetNeighbor(n);
				if(pNeighbor != NULL && !pNeighbor->IsClosed() && IsWalkable(pNeighbor, mWidth, mHeight))
				{
					float G = pNode->GetG() + GetG(pNode, pNeighbor);
					float H = GetH(pNeighbor, &mNodes[mEnd]);

					if(!pNeighbor->IsOpen())
					{
						// Set parent
						pNeighbor->SetParent(pNode);

						// Set Cost
						pNeighbor->SetG(G);
						pNeighbor->SetH(H);

						// Insert into open List
						InsertSortedByF(pNeighbor);
						pNeighbor->Open();
					}
					else if(pNeighbor->GetG() > G)
					{
						// Update Parent
						pNeighbor->SetParent(pNode);
						
						// Update cost
						pNeighbor->SetG(G);

						// Re-Insert to open list
						mOpenList.remove(pNeighbor);
						InsertSortedByF(pNeighbor);
					}
				}
			}
		}

		// Add Node to closed list
		mClosedList.push_front(pNode);
		pNode->Close();
	}

	// Check if we have a path
	if (found)
	{
		// Store path later perhaps
		// Clear path
		mPath.clear();

		// Construct path from end to start
		Node* pNode = &mNodes[mEnd];
		while(pNode != NULL)
		{
			mPath.push_front(pNode);
			pNode = pNode->GetParent();
		}
	}
}

void Graph::InsertSortedByG(Node* pNode)
{
	// Insert to open list sorted
	NodeList::iterator iter = mOpenList.begin();

	while(iter != mOpenList.end())
	{
		Node* node = *iter;
		if(node->GetG() > pNode->GetG())
		{
			break;
		}
		++iter;
	}
	mOpenList.insert(iter, pNode);
}

void Graph::InsertSortedByF(Node* pNode)
{
	// Insert to open list sorted
	NodeList::iterator iter = mOpenList.begin();

	while(iter != mOpenList.end())
	{
		Node* node = *iter;
		if(node->GetF() > pNode->GetF())
		{
			break;
		}
		++iter;
	}
	mOpenList.insert(iter, pNode);
}
