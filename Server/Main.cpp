#include <stdio.h>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" 
#include "News.h"
#include "Data.h"
#include "CurrentPlayers.h"
#include "FrontEnd.h"
#include "Defines.h"
#include "BaseConnection.h"
#include "Chat.h"
#include "GamePlay.h"
#include "TCPInterface.h"
#include <HTTPConnection.h> 

using namespace RakNet;

int main(int argc, char ** argv) {
	Data data;

	RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	Packet* packet = NULL;

	// Start Server
	SocketDescriptor sd(data.port, 0);
	peer->Startup(data.maxClients, &sd, 1);
	peer->SetMaximumIncomingConnections(data.maxClients);
	printf("Server Started on port: %d\n", data.port);

	CurrentPlayers currentPlayers(data, peer);
	FrontEnd frontEnd(currentPlayers, data);
	BaseConnection baseConnection(currentPlayers);
	GamePlay gamePlay(currentPlayers, data);

	currentPlayers.SetActiveMaps(gamePlay.GetActiveMaps());

	frontEnd.Init();
	gamePlay.Init();

	while (1) {
		gamePlay.Update();

		for (packet = peer->Receive(); packet;
				peer->DeallocatePacket(packet), packet = peer->Receive()) {
			frontEnd.Update(packet, peer);
			baseConnection.Update(packet, peer);
			gamePlay.UpdateRaknet(packet, peer);
		}

		currentPlayers.ConnectionUpdate();
	}

	// Close connection
	RakNet::RakPeerInterface::DestroyInstance(peer);
}
