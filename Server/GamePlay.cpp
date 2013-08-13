#include "GamePlay.h"

GamePlay::GamePlay(CurrentPlayers& currentPlayers, Data& data)
:mChat(currentPlayers)
,mCurrentPlayers(currentPlayers)
,mSavePlayers(currentPlayers, data)
,mData(data)
,mPlayerPosition(currentPlayers)
,mInit(false)
,mActiveMaps(currentPlayers)
,mCommands(currentPlayers)
,mItemEqiups(currentPlayers)
,mNPC(mActiveMaps, mCurrentPlayers)
{

}

GamePlay::~GamePlay()
{

}

void GamePlay::Init()
{
	mInit = true;
}

void GamePlay::Update()
{
	mSavePlayers.Update();
}

void GamePlay::UpdateRaknet(Packet* packet, RakPeerInterface* peer)
{
	if(!mInit)
	{
		return;
	}

	mChat.Update(packet, peer);
	mPlayerPosition.UpdateRaknet(packet, peer);
	mActiveMaps.UpdateRaknet(packet, peer);
	mCommands.UpdateRaknet(packet, peer);
	mItemEqiups.UpdateRaknet(packet, peer);
	mNPC.UpdateRaknet(packet, peer);

	switch (packet->data[0])
	{
	case ID_GET_PLAYER_INFO:
		{
			BitStream bsIn(packet->data,packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			Player* player = mCurrentPlayers.GetPlayer(packet->guid);

			// Send them the player
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_GET_PLAYER_INFO);
			bsOut.Write( *player );
			peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0, packet->guid, false);

			printf("Player info requested with GUID: %s\n", packet->guid.ToString());

			// Let everyone else know that we joined :)
			mPlayerPosition.NewPlayer(player->GetMap(), (*player) ,packet, peer);

			break;
		}

	case ID_GET_ITEMS:
		{
			// Send the player the items he has in his inventory
			mCurrentPlayers.GetPlayer(packet->guid)->GetItems(packet, peer);
			break;
		}

	case ID_MAP_DATA:
		{
			MapData* mapData = NULL;
			BitStream bsIn(packet->data, packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			int map = mCurrentPlayers.GetPlayer(packet->guid)->GetMap();

			if(!mActiveMaps.DoesMapExist(map))
			{
				mapData = new MapData;;
				MapLoad mapLoad(*mapData, mActiveMaps);
				mapLoad.Load(map);
			}
			else
			{
				mapData = mActiveMaps.GetMap(map);
			}

			mapData->AddPlayer(packet->guid);

			// Send them the map
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_MAP_DATA);
			bsOut.Write( map );
			bsOut.Write( mapData->GetWidth() );
			bsOut.Write( mapData->GetHeight() );
			bsOut.Write( mapData->GetTileAmount() );
			
			mapData->SendInfo(bsOut, packet);

			peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0, packet->guid, false);
			break;
		}

	case ID_GET_ACTIVE_PLAYERS:
		{
			BitStream bsIn(packet->data, packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			int map = 0;
			bsIn.Read(map);

			list<Player>& players = mCurrentPlayers.GetList();

			// Send the client all the active players on the same map as them
			for(list<Player>::iterator iter = players.begin(); iter != players.end(); ++iter)
			{
				// Dont send our own info back to ourselves
				if(iter->GetGUID() == packet->guid)
				{
					continue;
				}

				// If player is on the same map
				if(iter->GetMap() == map)
				{	
					// Send that player the position info
					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID)ID_GET_PLAYER);
					bsOut.Write(*iter);
					peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->guid,false);
				}
			}
			break;
		}

		case ID_REMOVE_ITEM:
		{
			printf("Player Used Item\n");

			BitStream bsIn(packet->data, packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			int itemNumber = 0;
			bsIn.Read(itemNumber);

			mCurrentPlayers.GetPlayer(packet->guid)->RemoveItem(itemNumber);

			break;
		}

		case ID_ADD_HEALTH:
		{
			BitStream bsIn(packet->data, packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			int health = 0;
			bsIn.Read(health);
		
			mCurrentPlayers.GetPlayer(packet->guid)->AddHealth(health);
			break;
		}

		case ID_ADD_MANA:
		{
			BitStream bsIn(packet->data, packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			int mana = 0;
			bsIn.Read(mana);
		
			mCurrentPlayers.GetPlayer(packet->guid)->AddMana(mana);
			break;
		}
	}
}
