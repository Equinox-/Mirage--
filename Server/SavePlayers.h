#ifndef _SAVEPLAYERS_H
#define _SAVEPLAYERS_H

#include <time.h> 
#include "CurrentPlayers.h"
#include "Data.h"

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class SavePlayers
{
public:
	SavePlayers(CurrentPlayers& currentPlayers, Data& data);
	~SavePlayers();

	void Update();

private:
	clock_t mStartTime;
	clock_t mTimePassed;
	double mUpdateTimer;

	Data& mData;
	CurrentPlayers& mCurrentPlayers;
};

#endif