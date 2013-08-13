#include "Raknet.h"

Raknet::Raknet()
:mClientID(0)
,mConnected(false)
,mIP(NULL)
{
	mPeer = RakNet::RakPeerInterface::GetInstance();
}

Raknet::~Raknet()
{
	RakNet::RakPeerInterface::DestroyInstance(mPeer);
}

void Raknet::Connect()
{
	// Get the ip address
	const char* mIP = IniFile_GetString("SERVER_IP", "127.0.0.1");
	int port = IniFile_GetInt("SERVER_PORT", 1337);
	mPeer->Startup(1,&mSD, 1);
	mPeer->Connect(mIP, port, 0,0);
}


void Raknet::Disconnect()
{
	// Tell Server we exited
	BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_DISCONNECTION_NOTIFICATION);
	mPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,mServerAddress,false);
}