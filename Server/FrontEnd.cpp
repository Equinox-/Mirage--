#include "FrontEnd.h"

FrontEnd::FrontEnd(CurrentPlayers& currentPlayers, Data& data)
:mNewsTimer(0)
,mStartTime(clock())
,mUpdateTimer(0.0f)
,mCurrentPlayers(currentPlayers)
,mData(data)
{

}

FrontEnd::~FrontEnd()
{

}

void FrontEnd::Init()
{
	// Get the news
	mNews.ReadNews();
}

void FrontEnd::Update(Packet* packet, RakPeerInterface* peer)
{
	// Update the timer
	mTimePassed = clock() - mStartTime;
	mUpdateTimer = mTimePassed / (double)CLOCKS_PER_SEC;

	// Update the server info every so often
	if(mUpdateTimer > mData.updateTimer)
	{
		mNews.ReadNews();
		mStartTime = clock();
	}

	switch (packet->data[0])
	{
	case ID_GET_NEWS:
		{
			// Send them the news
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_GET_NEWS);
			bsOut.Write(mNews.GetNews());
			peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);

			printf("Sent out News\n");
			break;
		}

	case ID_NEW_ACCOUNT:
		{
			RakString username;
			RakString email;
			RakString password;
			RakString ip;

			// Read the info
			BitStream bsIn(packet->data,packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(username);
			bsIn.Read(password);
			bsIn.Read(email);
			
			ip = packet->systemAddress.ToString(true, ':');

			// Create the account
			Player temp(username.C_String(), email.C_String(), password.C_String(), ip.C_String());
			int success = temp.Create();

			if(success)
			{
				// Send back a confirmation the acc was created
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_NEW_ACCOUNT);
				peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);

				printf("New Account Created. Name: %s\n", username.C_String());
			}
			else
			{
				// Tell them the account already exists
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_ACCOUNT_TAKEN);
				peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);

				printf("Account already taken. Name: %s\n", username.C_String());
			}
			break;
		}

	case ID_LOGIN:
		{
			RakString username;
			RakString password;

			// Read the info
			BitStream bsIn(packet->data,packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(username);
			bsIn.Read(password);

			bool success = mCurrentPlayers.Login(username.C_String(), password.C_String(), packet->guid);

			RakNet::BitStream bsOut;

			if(success)
			{
				if(!mCurrentPlayers.GetPlayer(packet->guid)->GetBanned())
				{
					// Did we login successfully? tell the client
					printf("%s Logged in with GUID: %s\n" , username.C_String(), packet->guid.ToString());
					bsOut.Write((RakNet::MessageID)ID_LOGIN);
					peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
				}
				else
				{
					// User banned
					printf("Banned user %s GUID: %s\n" , username.C_String(), packet->guid.ToString());
					bsOut.Write((RakNet::MessageID)ID_USER_BANNED);
					peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
				}
			}
			else
			{
				printf("%s Failed password check\n" , username.C_String());
				bsOut.Write((RakNet::MessageID)ID_LOGIN_WRONG_PASS);
				peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
			}
			break;
		}
	}
}