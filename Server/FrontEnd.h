#ifndef _FRONTEND_H
#define _FRONTEND_H

#include "Defines.h"
#include <time.h> 
#include "News.h"
#include "Player.h"
#include "CurrentPlayers.h"
#include "Data.h"

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class FrontEnd
{
public:
	FrontEnd(CurrentPlayers& currentPlayers, Data& data);
	~FrontEnd();

	void Init();
	void Update(Packet* packet, RakPeerInterface* peer);

private:
	News mNews;
	float mNewsTimer;
	clock_t mStartTime;
	clock_t mTimePassed;
	double mUpdateTimer;

	CurrentPlayers& mCurrentPlayers;
	Data& mData;

	
};

#endif