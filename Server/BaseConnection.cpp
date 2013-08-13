#include "BaseConnection.h"

BaseConnection::BaseConnection(CurrentPlayers& currentPlayers) :
		mCurrentPlayers(currentPlayers), mActiveConnections(0) {

}

BaseConnection::~BaseConnection() {

}

void BaseConnection::Update(Packet* packet, RakPeerInterface* peer) {
	switch (packet->data[0]) {
	case ID_REMOTE_DISCONNECTION_NOTIFICATION: {
		printf("A client has disconnected.\n");
		break;
	}
	case ID_REMOTE_CONNECTION_LOST: {
		printf("A client has lost the connection 1.\n");
		BitStream bsIn(packet->data, packet->length, false);

		Player* player = mCurrentPlayers.GetPlayer(packet->guid);
		player->Save();
		mCurrentPlayers.Disconnect(packet->guid);

		--mActiveConnections;

		break;
	}
	case ID_REMOTE_NEW_INCOMING_CONNECTION: {
		printf("A client has connected.\n");
		++mActiveConnections;
		break;
	}
	case ID_CONNECTION_REQUEST_ACCEPTED: {
		printf("Our connection request has been accepted.\n");

		++mActiveConnections;
		break;
	}
	case ID_NEW_INCOMING_CONNECTION: {
		printf("A connection is incoming.\n");
		++mActiveConnections;

		break;
	}
	case ID_NO_FREE_INCOMING_CONNECTIONS: {
		printf("The server is full.\n");

		break;
	}
	case ID_DISCONNECTION_NOTIFICATION: {
		printf("A client has disconnected.\n");

		BitStream bsIn(packet->data, packet->length, false);
		Player* player = mCurrentPlayers.GetPlayer(packet->guid);
		player->Save();
		mCurrentPlayers.Disconnect(packet->guid);

		--mActiveConnections;
		break;
	}
	case ID_CONNECTION_LOST: {
		printf("A client lost the connection. 2\n");
		BitStream bsIn(packet->data, packet->length, false);
		Player* player = mCurrentPlayers.GetPlayer(packet->guid);
		player->Save();
		mCurrentPlayers.Disconnect(packet->guid);

		--mActiveConnections;
		break;
	}
	}

	char temp[CHAR_MAX];
	sprintf(temp, "Eclipse++ Server - Active Connections: %d",
			mActiveConnections);
	// TODO make magic
	//SetConsoleTitleA(temp);
}
