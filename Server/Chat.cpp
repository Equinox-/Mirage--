#include "Chat.h"

Chat::Chat(CurrentPlayers& currentPlayers)
:mCurrentPlayers(currentPlayers)
{

}

Chat::~Chat()
{

}

void Chat::Init()
{

}

void Chat::Update(Packet* packet, RakPeerInterface* peer)
{
	switch (packet->data[0])
	{
	case ID_CHATMESSAGE:
		{
			RakString message;

			// Read the info
			BitStream bsIn(packet->data,packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(message);

			// Send message to all users
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_CHATMESSAGE);
			bsOut.Write(message);
			bsOut.Write(mCurrentPlayers.GetPlayer(packet->guid)->GetUsername());
			peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,UNASSIGNED_RAKNET_GUID, true);
			printf("Chat message sent to all users\n");
		}
	}
}