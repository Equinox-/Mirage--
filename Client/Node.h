#ifndef INCLUDED_AINODE_H
#define INCLUDED_AINODE_H

#include <SGE.h>
using namespace SGE;

#include <list>

class Node
{
public:
	static const int kMaxNeighbors = 8;
	static const int kInvalidID = -1;

protected:
	Node* mpNeighbors[kMaxNeighbors];
	Node* mpParent;

	int mId;
	SVector2 mPosition;

	float mG;
	float mH;

	bool mOpen;
	bool mClosed;
	bool mWalkable;
	float mCost;


public:
	Node();

	Node* GetNeighbor(int index) const		{ return mpNeighbors[index]; }
	Node* GetParent() const					{ return mpParent;}
	int GetId() const						{ return mId; }
	const SVector2& GetPosition() const		{ return mPosition; }
	float GetF() const						{ return mG + mH; }
	float GetG() const						{ return mG; }
	float GetH() const						{ return mH; }
	float GetCost()	const					{ return mCost; }

	void SetNeighbor(int index, Node* n)	{ mpNeighbors[index] = n; }
	void SetParent(Node* p)					{ mpParent = p; };
	void SetId(int id)						{ mId = id; }
	void SetPosition(const SVector2& pos)	{ mPosition = pos; }
	void SetWalkable(const bool trueFalse)	{ mWalkable = trueFalse; }
	void SetG(float g)						{ mG = g; }
	void SetH(float h)						{ mH = h; }
	void SetCost(float cost)				{ mCost = cost; }

	bool IsOpen() const						{ return mOpen; }
	bool IsClosed() const					{ return mClosed; }
	bool IsWalkable() const					{ return mWalkable; }

	void Open()								{ mOpen = true; }
	void Close()							{ mClosed = true; }
	void Reset();


};

typedef std::list<Node*> NodeList;

#endif // #ifndef INCLUDED_AINODE_H