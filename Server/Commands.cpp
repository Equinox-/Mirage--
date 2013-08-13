#include "Commands.h"


Commands::Commands(CurrentPlayers& currentPlayers)
:mCurrentPlayers(currentPlayers)
{

}

Commands::~Commands()
{

}

void Commands::UpdateRaknet(Packet* packet, RakPeerInterface* peer)
{
	switch (packet->data[0])
	{
	case ID_SET_SPRITE:
		{
			int spriteNumber = 0;
			BitStream bsIn(packet->data, packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			bsIn.Read(spriteNumber);

			Player* player = mCurrentPlayers.GetPlayer(packet->guid);

			if(player->GetAccess() > 0)
			{
				// Save the sprite change
				player->SetSpriteNumber(spriteNumber);

				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_SET_SPRITE);
				bsOut.Write(spriteNumber);
				printf("user changed sprite\n");
				peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0, packet->guid,false);
			}
			break;
		}

		case ID_SET_SPRITE_NAME:
		{
			char name[CHAR_MAX];
			int spriteNumber = 0;

			BitStream bsIn(packet->data, packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			bsIn.Read(name);
			bsIn.Read(spriteNumber);
			
			Player* player = mCurrentPlayers.GetPlayer(packet->guid);

			if(player->GetAccess() > 0)
			{
				player = mCurrentPlayers.GetPlayer(name);
				if(!player)
					return;

				// Save the sprite change
				player->SetSpriteNumber(spriteNumber);

				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_SET_SPRITE);
				bsOut.Write(spriteNumber);
				printf("user changed sprite\n");
				peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0, player->GetGUID(),false);
			}
			break;
		}

	}
}
