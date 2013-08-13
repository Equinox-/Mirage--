#ifndef _WALKFRAMES_H
#define _WALKFRAMES_H

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

struct WalkFrames
{
	WalkFrames()
		:WalkDownStart(0)
		,WalkDownEnd(0)
		,WalkLeftStart(0)
		,WalkLeftEnd(0)
		,WalkUpStart(0)
		,WalkUpEnd(0)
	{}

	int WalkDownStart;
	int WalkDownEnd;
	int WalkLeftStart;
	int WalkLeftEnd;
	int WalkUpStart;
	int WalkUpEnd;
};

#endif