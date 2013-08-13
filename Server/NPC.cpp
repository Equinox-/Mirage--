#include "NPC.h"

NPC::NPC(ActiveMaps& activeMaps, CurrentPlayers& currentPlayers)
:mActiveMaps(activeMaps)
,mCurrentPlayers(currentPlayers)
{

}

NPC::~NPC()
{

}

void NPC::UpdateRaknet(Packet* packet, RakPeerInterface* peer)
{
	switch (packet->data[0])
	{
	case ID_NPC_POSITION:
		{
			int arrayIndex = 0;
			int pathIndex = 0;
			SVector2 position(0.0f, 0.0f);

			BitStream bsIn(packet->data, packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			bsIn.Read(arrayIndex);
			bsIn.Read(pathIndex);
			bsIn.Read(position);

			int mapID = mCurrentPlayers.GetPlayer(packet->guid)->GetMap();
			MapData* map = mActiveMaps.GetMap(mapID);

			map->SetNPCPath(arrayIndex, pathIndex);
			map->SetNPCPosition(arrayIndex, position);
			break;
		}

	case ID_NPC_NEW_PATH:
		{
			int npcIndex;
			int endIndex;
			SVector2 position;

			BitStream bsIn(packet->data, packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(npcIndex);
			bsIn.Read(endIndex);
			bsIn.Read(position);

			int map = mCurrentPlayers.GetPlayer(packet->guid)->GetMap();

			MapData* tempMap = mActiveMaps.GetMap(map);
			tempMap->SetNPCEndIndex(npcIndex, endIndex);
			tempMap->SetNPCPosition(npcIndex, position);
			mCurrentPlayers.SendNewPath(map, npcIndex, endIndex, position);
			break;
		}

		case ID_NPC_STOP:
		{
			int npcIndex = 0;
			bool stop = false;

			BitStream bsIn(packet->data, packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(npcIndex);
			bsIn.Read(stop);

			if(stop)
			printf("NPC Stopped\n");
			else
				printf("NPC Started\n");
			int map = mCurrentPlayers.GetPlayer(packet->guid)->GetMap();
			mActiveMaps.GetMap(map)->SetNPCStop(npcIndex, stop);
			mCurrentPlayers.SendNPCStop(packet, peer, npcIndex, stop, map);
			break;
		}
	}
}