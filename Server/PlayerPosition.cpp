#include "PlayerPosition.h"

PlayerPosition::PlayerPosition(CurrentPlayers& currentPlayers)
:mCurrentPlayers(currentPlayers)
{

}

PlayerPosition::~PlayerPosition()
{

}

void PlayerPosition::Update()
{

}


void PlayerPosition::UpdateRaknet(Packet* packet, RakPeerInterface* peer)
{
	switch (packet->data[0])
	{
	case ID_UPDATE_POSITION:
		{
			SVector2 position;
			Direction direction;

			BitStream bsIn(packet->data, packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			bsIn.Read(position);
			bsIn.Read(direction);

			Player* player = mCurrentPlayers.GetPlayer(packet->guid);
			player->SetPosition(position);
			player->SetDirection(direction);

			int map = player->GetMap();
			RakNetGUID guid = player->GetGUID();
			list<Player>& players = mCurrentPlayers.GetList();

			printf("Player Position recieved from GUID: %s\n", packet->guid.ToString());
			// Iterate through all the active players and send there positions if on the same map as other people
			for(list<Player>::iterator iter = players.begin(); iter != players.end(); ++iter)
			{
				// Dont send our own info back to ourselves
				if(iter->GetGUID() == guid)
				{
					continue;
				}

				// If player is on the same map
				if(iter->GetMap() == map)
				{	
					// Send that player the position info
					// Remember we are sending to the iterator. Not taking the iterators info
					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID)ID_UPDATE_POSITION);
					bsOut.Write(packet->guid);
					bsOut.Write(position);
					bsOut.Write(direction);
					printf("Position sent from GUID: %s to GUID: %s\n", packet->guid.ToString(), iter->GetGUID().ToString());
					peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,iter->GetGUID(),false);
				}
			}

			break;
		}
	}
}

void PlayerPosition::NewPlayer(int map, Player& player, Packet* packet, RakPeerInterface* peer)
{
	list<Player>& players = mCurrentPlayers.GetList();

	// Iterate through all the active players and send there positions if on the same map as other people
	for(list<Player>::iterator iter = players.begin(); iter != players.end(); ++iter)
	{
		// If player is on the same map
		if(iter->GetMap() == map)
		{	
			// Dont send our own info back to ourselves
			if(iter->GetGUID() == player.GetGUID())
			{
				continue;
			}

			// Send that player the position info
			// Remember we are sending to the iterator. Not taking the iterators info
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_GET_PLAYER);
			bsOut.Write(player);
			peer->Send(&bsOut, HIGH_PRIORITY,RELIABLE_ORDERED, 0, (*iter).GetGUID(),false);
		}
	}
}