#include "OtherPlayers.h"

OtherPlayers::OtherPlayers(Raknet& raknet, PlayerInfo& playerInfo)
:mRaknet(raknet)
,mPlayerInfo(playerInfo)
{

}

void OtherPlayers::Init()
{
	mFont.Load(17, true);
	mFont.SetColor(255,255,255);
}

OtherPlayers::~OtherPlayers()
{

}

void OtherPlayers::Update(float deltaTime)
{
	for(std::vector<Player*>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		(*iter)->Update(deltaTime);
	}
}

void OtherPlayers::Render(SVector2 offset)
{
	for(std::vector<Player*>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		Player* player = (*iter);
		player->Render(offset);

		const int kTextureWidth = player->mSprite.GetWidth();
		const int kTextureHeight = player->mSprite.GetHeight();
		const SVector2 ownerPos(player->mPlayerInfo.GetPosition());
		const SVector2 renderPos(ownerPos.x - (kTextureWidth * 0.5f), ownerPos.y - kTextureHeight);

		SVector2 newPosition = renderPos + offset;

		mFont.Print(player->mPlayerInfo.GetUsername(), newPosition.x, newPosition.y - 20);
	}
}

void OtherPlayers::UpdateRaknet()
{
	switch (mRaknet.mPacket->data[0])
	{
	case ID_UPDATE_POSITION:
		{
			RakNet::BitStream bsIn(mRaknet.mPacket->data,mRaknet.mPacket->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			SVector2 position(0.0f, 0.0f);
			RakNetGUID guid;
			Direction direction;

			bsIn.Read(guid);
			bsIn.Read(position);
			bsIn.Read(direction);

			for(std::vector<Player*>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
			{
				if( (*iter)->mPlayerInfo.GetGUID() == guid)
				{
					(*iter)->SetNewPosition(position);
					(*iter)->mPlayerInfo.SetDirection(direction);
				}
			}
			break;
		}

	case ID_GET_PLAYER:
		{
			RakNet::BitStream bsIn(mRaknet.mPacket->data,mRaknet.mPacket->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			PlayerInfo playerInfo;
			bsIn.Read(playerInfo);

			Player* player = new Player(playerInfo);
			player->Init();

			// Add player to active user list
			mPlayers.push_back(player);
			break;
		}

		case ID_PLAYER_LEFT:
		{
			RakNetGUID guid;

			RakNet::BitStream bsIn(mRaknet.mPacket->data,mRaknet.mPacket->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(guid);

			// Remove player from the list
			for(std::vector<Player*>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
			{
				if( (*iter)->mPlayerInfo.GetGUID() == guid)
				{
					iter = mPlayers.erase(iter);

					
					return;
				}
			}

			break;
		}
	}
}

void OtherPlayers::GetPlayerActivePlayers()
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GET_ACTIVE_PLAYERS);
	bsOut.Write(mPlayerInfo.GetMap());
	mRaknet.mPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,mRaknet.mServerAddress,false);
}

void OtherPlayers::RemovePlayer(RakNetGUID& guid)
{
	for(std::vector<Player*>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		if((*iter)->mPlayerInfo.GetGUID() == guid)
		{
			iter = mPlayers.erase(iter);
			return;
		}
	}
}


Player* OtherPlayers::GetPlayer(char* name)
{
	for(std::vector<Player*>::iterator iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		if( strcmp( (*iter)->mPlayerInfo.GetUsername(), name) == 0)
		{
			return (*iter);
		}
	}
}