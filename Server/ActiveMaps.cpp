#include "ActiveMaps.h"

ActiveMaps::ActiveMaps(CurrentPlayers& currentPlayers)
:mCurrentPlayers(currentPlayers)
{

}


ActiveMaps::~ActiveMaps()
{

}

void ActiveMaps::RemoveMap(int id)
{
	
}

bool ActiveMaps::DoesMapExist(int id)
{
	for(std::vector<MapData*>::iterator iter = mMaps.begin(); iter != mMaps.end(); ++iter)
	{
		if(id == (*iter)->GetMapNumber())
		{
			return true;
		}
	}

	return false;
}

MapData* ActiveMaps::GetMap(int map)
{
	bool mapExist = DoesMapExist(map);
	
	if(!mapExist)
	{
		assert (printf("Error trying to Get Map that does not exist"));
		return NULL;
	}

	for(std::vector<MapData*>::iterator iter = mMaps.begin(); iter != mMaps.end(); ++iter)
	{
		if(map == (*iter)->GetMapNumber())
		{
			return (*iter);
		}
	}
}

void ActiveMaps::UpdateRaknet(Packet* packet, RakPeerInterface* peer)
{
	switch (packet->data[0])
	{
	case ID_PICKUP_ITEM:
		{
			printf("Item Pickup Requested\n");

			int item = 0;
			int map = 0;
			int itemIndex = 0;

			// Read the info
		BitStream bsIn(packet->data,packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(item);
			bsIn.Read(itemIndex);
			bsIn.Read(map);

			MapData* myMap = GetMap(map);
			GroundTile* tile = myMap->FindItem(item);
			int itemType = 0;

			ItemBase* itemClass = ReadItem(tile->GetItemNumber(), itemType);

			// If this item is actually still availible to pickup
			if(tile->GetActive())
			{
				// Send the player the info that they can pick it up :O
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_PICKUP_ITEM);
				bsOut.Write(itemType);
				
				SendItemData(itemClass, bsOut);
				itemClass->SendData(bsOut);

				peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->guid,false);
			}

			// Not active anymore
			tile->SetActive(false);
			mCurrentPlayers.SendItemTaken(itemIndex, map);
			mCurrentPlayers.GetPlayer(packet->guid)->AddItem(tile->GetItemNumber(), 1);
			if(itemClass)
			{
				delete itemClass;
				itemClass = NULL;
			}

			break;
		}

	case ID_DROP_ITEM:
		{
			BitStream bsIn(packet->data,packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			int map = 0;
			int imageNumber = 0;
			int itemNumber = 0;
			int index = 0;		

			bsIn.Read(map);
			bsIn.Read(imageNumber);
			bsIn.Read(itemNumber);
			bsIn.Read(index);

			int id = index;

			// Set up the active map tile
			GetMap(map)->AddItem(imageNumber, itemNumber, index);
			mCurrentPlayers.ItemDropped(packet, peer, map, imageNumber, index);
			break;
		}
	}
}
