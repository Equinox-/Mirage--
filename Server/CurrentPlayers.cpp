#include "CurrentPlayers.h"

CurrentPlayers::CurrentPlayers(Data& data, RakPeerInterface* peer)
:mStartTime(clock())
,mUpdateTimer(0.0f)
,mData(data)
,mPeer(peer)
{

}

CurrentPlayers::~CurrentPlayers()
{

}

bool CurrentPlayers::Login(const char* username, const char* password, RakNetGUID guid)
{
	bool success = false;

	Player temp(username);
	temp.Load();
	temp.SetGUID(guid);

	// If password matches
	if(strcmp(temp.GetPassword(), password) == 0)
	{
		bool alreadyLogged = false;

		// Check if are already logged in
		for(list<Player>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
		{
			if(iter->GetGUID() == guid)
			{
				alreadyLogged = true;
				break;
			}
		}

		if(!alreadyLogged)
		{
			mPlayers.push_back(temp);
			success = true;
		}
	}
	else
	{
		success = false;
	}

	return success;
}

void CurrentPlayers::Disconnect(RakNetGUID guid)
{
	// Let all the current players know a player has left
	for(list<Player>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_PLAYER_LEFT);
		bsOut.Write(guid);
		mPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,iter->GetGUID(),false);
	}

	// Remove player from the list
	for(list<Player>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		if(iter->GetGUID() == guid)
		{
			mActiveMaps->GetMap(iter->GetMap())->PlayerDisconnect(guid, mPeer);
			iter = mPlayers.erase(iter);
			
			return;
		}
	}
}

void CurrentPlayers::ConnectionUpdate()
{
	// Update the timer
	mTimePassed = clock() - mStartTime;
	mUpdateTimer = mTimePassed / (double)CLOCKS_PER_SEC;

	// Let all the clients know they are still connected
	if(mUpdateTimer > mData.connectionUpdateTimer)
	{
		for(list<Player>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
		{
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_STILLCONNECTED);
			mPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,iter->GetGUID(),false);
		}
		mStartTime = clock();

		printf("Updated Connections \n");
	}
}


Player* CurrentPlayers::GetPlayer(RakNetGUID& guid)
{
	for(list<Player>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		if(iter->GetGUID() == guid)
		{
			return &(*iter);
		}
	}
	
	return NULL;
}

Player* CurrentPlayers::GetPlayer(char* name)
{
	for(list<Player>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		if(strcmp((iter)->GetUsername(), name) == 0)
		{
			return &(*iter);
		}
	}

	return NULL;
}

void CurrentPlayers::SavePlayers()
{
	int count = 0;
	for(list<Player>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		iter->Save();
		++ count;
	}

	printf("%d Players Saved\n", count);
}

void CurrentPlayers::SendItemTaken(int itemIndex, int map)
{
	for(list<Player>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		// If a player is on the same map
		if(iter->GetMap() == map)
		{
			// Tell them the item is taken
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_ITEM_TAKEN);
			bsOut.Write(itemIndex);
			mPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0, iter->GetGUID(), false);
		}
	}
}

void CurrentPlayers::SendNewPath(int map, int npcSlot, int endIndex, SVector2 position)
{
	for(list<Player>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		// If a player is on the same map
		if(iter->GetMap() == map)
		{
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_NPC_NEW_PATH);
			bsOut.Write(npcSlot);
			bsOut.Write(endIndex);
			bsOut.Write(position);
			
			mPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0, iter->GetGUID(),false);
		}
	}
}

void CurrentPlayers::ItemDropped(Packet* packet, RakPeerInterface* peer, int map, int imageNumber, int index)
{
	for(list<Player>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		// If a player is on the same map
		if(iter->GetMap() == map)
		{
			// Tell them the item is dropped
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_DROP_ITEM);

			bsOut.Write(imageNumber);
			bsOut.Write(index);

			mPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0, iter->GetGUID(), false);
		}
	}
}

void CurrentPlayers::SendNPCStop(Packet* packet, RakPeerInterface* peer, int arrayIndex, bool stop, int map)
{
	for(list<Player>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		// If a player is on the same map
		if(iter->GetMap() == map)
		{
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_NPC_STOP);
			bsOut.Write(arrayIndex);
			bsOut.Write(stop);
			mPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,iter->GetGUID(),false);
		}
	}
}