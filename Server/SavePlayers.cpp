#include "SavePlayers.h"

SavePlayers::SavePlayers(CurrentPlayers& currentPlayers, Data& data)
:mCurrentPlayers(currentPlayers)
,mData(data)
{

}


SavePlayers::~SavePlayers()
{

}

void SavePlayers::Update()
{
	// Update the timer
	mTimePassed = clock() - mStartTime;
	mUpdateTimer = mTimePassed / (double)CLOCKS_PER_SEC;

	// Let all the clients know they are still connected
	if(mUpdateTimer > mData.saveUpdateTimer)
	{
		mCurrentPlayers.SavePlayers();
		mStartTime = clock();
	}
}