#include "Commands.h"

Commands::Commands(Raknet& raknet, PlayerInfo& playerInfo, Character& character, Map& map)
:mRaknet(raknet)
,mPlayerInfo(playerInfo)
,mCharacter(character)
,mMap(map)
{

}


Commands::~Commands()
{

}

void Commands::UpdateRaknet()
{
	switch (mRaknet.mPacket->data[0])
	{
	case ID_SET_SPRITE:
		{
			int sprite = 0;
			RakNet::BitStream bsIn(mRaknet.mPacket->data,mRaknet.mPacket->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(sprite);

			mPlayerInfo.SetSpriteNumber(sprite);
			mCharacter.UpdateSprite();
			break;
		}
	}
}
