#include "Inventory.h"

Inventory::Inventory(int inventorySlots, Raknet& raknet, Map& map,
		PlayerInfo& playerInfo, Character& character) :
		mInventorySlots(inventorySlots), mRaknet(raknet), mMap(map), mDragSprite(
				SVector2((float) SGE::Graphics_WindowWidth() - 300.0, 290.0f),
				false), mPlayerInfo(playerInfo), mCharacter(character) {

}

Inventory::~Inventory() {

}

void Inventory::Init() {
	mDragSprite.Init("GUI/Ingame/inventory2.png");

	// Ask for our current items
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID) ID_GET_ITEMS);
	mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
			mRaknet.mServerAddress, false);

	mFont.Load(10, true);
	mFont.SetColor(255, 0, 0);
}

void Inventory::AddItem(ItemBase* item) {
	if (mItems.size() < mInventorySlots) {
		mItems.push_back(item);
	}
}

void Inventory::Update(float deltaTime) {
	mDragSprite.Update(deltaTime);

	int x = Input_GetMouseScreenX();
	int y = Input_GetMouseScreenY();
	int dragSpriteX = mDragSprite.GetPosition().x;
	int dragSpriteY = mDragSprite.GetPosition().y;
	int width = mDragSprite.GetSpriteWidth();
	int height = mDragSprite.GetSpriteHeight();

	// Check for item use
	if (((x > dragSpriteX) && x < dragSpriteX + width)
			&& (y > dragSpriteY && y < dragSpriteY + height)
			&& mDragSprite.GetVisible()) {
		bool mouseDown = Input_IsMousePressed(Mouse::LBUTTON);
		SVector2 position = mDragSprite.GetPosition();
		float x = Input_GetMouseScreenX() - position.x;
		float y = Input_GetMouseScreenY() - position.y - 32;

		// Exit Button
		if (mouseDown && x > 264 && x < 286 && y < 0) {
			mDragSprite.ToggleVisible();
		}

		int itemSlot = -1;

		// Figure out what item we are selecting
		int xSlot = -1;

		if (x > 27 && x < 70)
			xSlot = 0;
		if (x > 78 && x < 120)
			xSlot = 1;
		if (x > 127 && x < 170)
			xSlot = 2;
		if (x > 178 && x < 218)
			xSlot = 3;
		if (x > 226 && x < 270)
			xSlot = 4;

		int ySlot = -1;

		if (y > -4 && y < 34)
			ySlot = 0;
		if (y > 46 && y < 85)
			ySlot = 1;
		if (y > 93 && y < 135)
			ySlot = 2;
		if (y > 143 && y < 183)
			ySlot = 3;
		if (y > 192 && y < 234)
			ySlot = 4;
		if (y > 242 && y < 282)
			ySlot = 5;

		if (xSlot >= 0 && ySlot >= 0) {
			itemSlot = xSlot + ySlot * 5;
		}

		if (mouseDown) {
			if (itemSlot >= 0 && itemSlot < mItems.size()) {
				mItems[itemSlot]->UseItem(mPlayerInfo, mRaknet);
				mItems.erase(mItems.begin() + itemSlot);
			}
		}

		if (Input_IsMousePressed(Mouse::RBUTTON)) {
			// Delete Item in slot
			if (itemSlot >= 0 && itemSlot < mItems.size()) {
				ItemBase* item = mItems[itemSlot];
				int imageNumber = item->GetImageNumber();
				mItems.erase(mItems.begin() + itemSlot);

				// Let the server know we droped a item
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID) ID_DROP_ITEM);

				bsOut.Write(mPlayerInfo.GetMap());
				bsOut.Write(imageNumber);
				bsOut.Write(item->GetItemNumber());
				bsOut.Write(mCharacter.GetTileIndex());

				mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
						mRaknet.mServerAddress, false);
			}
		}
	}
}

void SetValues(ItemBase* item, char* itemName, int value, bool questItem,
		int imageNumber, char* description, int itemNumber) {
	item->SetName(itemName);
	item->SetValue(value);
	item->SetQuest(questItem);
	item->SetImageNumber(imageNumber);
	item->SetDescription(description);
	item->SetItemNumber(itemNumber);
}

void Inventory::UpdateRaknet() {
	switch (mRaknet.mPacket->data[0]) {
	case ID_PICKUP_ITEM: {
		ItemBase* item;
		int itemType = 0;

		char itemName[CHAR_MAX];
		int value = 0;
		bool questItem = 0;
		int imageNumber = 0;
		char description[1000];
		int itemNumber = 0;

		RakNet::BitStream bsIn(mRaknet.mPacket->data, mRaknet.mPacket->length,
				false);
		bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		bsIn.Read(itemType);

		bsIn.Read(itemName);
		bsIn.Read(value);
		bsIn.Read(questItem);
		bsIn.Read(imageNumber);
		bsIn.Read(description);
		bsIn.Read(itemNumber);

		switch (itemType) {
		case potion: {
			item = new Potion;
			break;
		}

		case weapon: {
			item = new Weapon;
			break;
		}

		case shield: {
			item = new Shield;
			break;
		}

		case helmet: {
			item = new Helmet;
			break;
		}

		case armor: {
			item = new Armor;
			break;
		}

		case boots: {
			item = new Boots;
			break;
		}

		case necklace: {
			item = new Necklace;
			break;
		}

		case ring: {
			item = new Ring;
			break;
		}
		}

		SetValues(item, itemName, value, questItem, imageNumber, description,
				itemNumber);
		item->GetData(bsIn);
		AddItem(item);
		break;
	}
	}

}

void Inventory::Render() {
	if (mDragSprite.GetVisible()) {
		mDragSprite.Render();

		SVector2 position = mDragSprite.GetPosition();
		float x = 32;
		float y = 33;

		for (std::vector<ItemBase*>::iterator iter = mItems.begin();
				iter != mItems.end(); ++iter) {
			Item* temp = mMap.GetItemSprite((*iter)->GetImageNumber());
			int num = (*iter)->GetImageNumber();
			temp->mSprite.SetPosition(position.x + x, position.y + y);
			temp->mSprite.Render();
			x += 50;

			if (x > 250) {
				y += 49;
				x = 32;
			}
		}

		char temp[CHAR_MAX];
		sprintf(temp, "%d", mPlayerInfo.GetGold());
		mFont.Print(temp, position.x + 180, position.y + 330);
	}
}

void Inventory::Unload() {
	for (std::vector<ItemBase*>::iterator iter = mItems.begin();
			iter != mItems.end(); ++iter) {
		delete (*iter);
		continue;
	}
}

