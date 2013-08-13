#include "Equip.h"

Equip::Equip(Raknet& raknet, PlayerInfo& playerInfo, Inventory& inventory)
:mDragSprite(SVector2(800.0f, 130.0f), false)
,mRaknet(raknet)
,mPlayerInfo(playerInfo)
,mInventory(inventory)
,mStats()
{

}

Equip::~Equip()
{

}

void Equip::Init()
{
	mDragSprite.Init("GUI/Ingame/Eqiupment.png");

	char temp[CHAR_MAX];

	for(int a = 0; a < 8; ++a)
	{
		int item = mPlayerInfo.GetEqiupItemImage(a);
		if(item > 0)
		{
			sprintf(temp, "Items/Item_%d.png", item);
			mSprite[a].Load(temp);
		}
	}

	mStats.Init();

	mFont.Load(15, true);
	mFont.SetColor(139,123,139);
}

void Equip::Update(float deltaTime)
{
	mDragSprite.Update(deltaTime);
	mStats.Update(deltaTime);

	mStats.SetPosition(mDragSprite.GetPosition() + SVector2(13.5f, 228.0f));

	SVector2 position = mDragSprite.GetPosition();
	float x = Input_GetMouseScreenX() - position.x;
	float y = Input_GetMouseScreenY() - position.y - 32;

	bool mouseDown = Input_IsMousePressed(Mouse::LBUTTON);

	// Exit Button
	if(mouseDown && x > 260 && x < 290 && y < 2)
	{
		mDragSprite.ToggleVisible();
	}

	// Stat Button
	if( (mouseDown && x > 77 && x < 220 && y > 168 && y < 187))
	{
		mStats.ToggleVisible();
	}
}

void Equip::Render()
{
	if(mDragSprite.GetVisible())
	{
		SVector2 position = mDragSprite.GetPosition();
		float x = Input_GetMouseScreenX() - position.x;
		float y = Input_GetMouseScreenY() - position.y - 32;

		int slot = -1;
		if(Input_IsMousePressed(Mouse::LBUTTON))
		{
			// Ring Left
			if( x > 226 && x < 269 && y > 93 && y < 138)
			{
				slot = ring2;
			}

			// Ring Right
			if( x > 226 && x < 269 && y > 144 && y < 187)
			{
				slot = ring;
			}

			// Right arm
			if( x > 177 && x < 219 && y > 119 && y < 162)
			{
				slot = shield;
			}

			// Left arm
			if( x > 78 && x < 118 && y > 120 && y < 163)
			{
				slot = weapon;
			}

			// Helmet
			if( x > 28 && x < 70 && y > -4 && y < 40)
			{
				slot = helmet;
			}

			// Armor
			if( x > 28 && x < 70 && y > 94 && y < 137)
			{
				slot = armor;
			}

			// Boots
			if( x > 28 && x < 70 && y > 143 && y < 189)
			{
				slot = boots;
			}

			// Necklace
			if( x > 28 && x < 70 && y > 45 && y < 88)
			{
				slot = necklace;
			}

			if(slot >= 0 && mInventory.GetSpaceLeft() > 0 && mPlayerInfo.GetEqiupItemImage(slot) > 0)
			{
				mSprite[slot].Unload();
				mPlayerInfo.SetEqiupItem(slot, 0, 0);

				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_UNEQIUP_ITEM);
				bsOut.Write(slot);
				mRaknet.mPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,mRaknet.mServerAddress,false);
			} 
		}

		mDragSprite.Render();
		mStats.Render();

		mSprite[weapon].SetPosition(position.x + 83, position.y + 155);
		mSprite[shield].SetPosition(position.x + 182, position.y + 155);
		mSprite[helmet].SetPosition(position.x + 32, position.y + 32);
		mSprite[armor].SetPosition(position.x + 32, position.y + 130);
		mSprite[boots].SetPosition(position.x + 32, position.y + 182);
		mSprite[necklace].SetPosition(position.x + 32, position.y + 81);
		mSprite[ring].SetPosition(position.x + 230, position.y + 132);
		mSprite[ring2].SetPosition(position.x + 230, position.y + 182);


		for(int a = 0; a < 8; ++a)
		{
			if(mPlayerInfo.GetEqiupItemImage(a) > 0)
			{
				mSprite[a].Render();
			}
		}


		char temp[CHAR_MAX];
		sprintf(temp, "MouseX: %f", x);
		Graphics_DebugText(temp, 5, 225, 0XFF0000);

		sprintf(temp, "MouseY: %f", y);
		Graphics_DebugText(temp, 5, 250, 0XFF0000);
		
		sprintf(temp, "%s", mPlayerInfo.GetUsername());
		mFont.Print(temp, (int)(position.x + 112.0f), (int)(position.y + 23.0f));

		if(mStats.GetVisible())
		{
			sprintf(temp, "%d", mPlayerInfo.GetStr());
			mFont.Print(temp, (int)(position.x + 210.0f), (int)(position.y + 288.0f));

			sprintf(temp, "%d", mPlayerInfo.GetDex());
			mFont.Print(temp, (int)(position.x + 210.0f), (int)(position.y + 305.0f));

			sprintf(temp, "%d", mPlayerInfo.GetInt());
			mFont.Print(temp, (int)(position.x + 210.0f), (int)(position.y + 322.0f));

			sprintf(temp, "%d", mPlayerInfo.GetAttack());
			mFont.Print(temp, (int)(position.x + 210.0f), (int)(position.y + 339.0f));

			sprintf(temp, "%d", mPlayerInfo.GetDefence());
			mFont.Print(temp, (int)(position.x + 210.0f), (int)(position.y + 356.0f));

			sprintf(temp, "%d", mPlayerInfo.GetCrit());
			mFont.Print(temp, (int)(position.x + 210.0f), (int)(position.y + 373.0f));
		}
	}
}

void Equip::UpdateRaknet()
{
	switch (mRaknet.mPacket->data[0])
	{
	case ID_EQIUP_ITEM:
		{
			int itemType = 0;
			int itemNumber = 0;
			int imageNumber = 0;

			RakNet::BitStream bsIn(mRaknet.mPacket->data,mRaknet.mPacket->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			bsIn.Read(itemType);
			bsIn.Read(itemNumber);
			bsIn.Read(imageNumber);

			char temp[CHAR_MAX];
			sprintf(temp, "Items/Item_%d.png", imageNumber);

			if(itemType != ring)
			{
				mSprite[itemType].Unload();
				mSprite[itemType].Load(temp);
			}
			else
			{
				// If the second ring is full swap out the first. If they are both full also swap out the first
				if(mPlayerInfo.GetEqiupItemImage(ring2) > 0)
				{
					mSprite[ring].Unload();
					mSprite[ring].Load(temp);
				}
				else // Second slot is not full fill that one
				{
					mSprite[ring2].Unload();
					mSprite[ring2].Load(temp);
				}
			}

			mPlayerInfo.SetEqiupItem(itemType, itemNumber, imageNumber);

			break;
		}

		case ID_ITEM_BONUS:
		{
			RakNet::BitStream bsIn(mRaknet.mPacket->data,mRaknet.mPacket->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			int str = 0;
			int dex = 0;
			int def = 0;
			int intStat = 0;
			int atk = 0;
			int defence = 0;
			int critChance = 0;
			int health = 0;
			int mana = 0;

			bsIn.Read(str);
			bsIn.Read(dex);
			bsIn.Read(def);
			bsIn.Read(intStat);
			bsIn.Read(atk);
			bsIn.Read(defence);
			bsIn.Read(critChance);
			bsIn.Read(health);
			bsIn.Read(mana);

			mPlayerInfo.AddStr(str);
			mPlayerInfo.AddDex(dex);
			mPlayerInfo.AddDef(def);
			mPlayerInfo.AddInt(intStat);
			mPlayerInfo.AddAtk(atk);
			mPlayerInfo.AddDef(defence);
			mPlayerInfo.AddCrit(critChance);
			mPlayerInfo.AddHealthBonus(health);
			mPlayerInfo.AddManaBonus(mana);

			break;
		}
	}
}
