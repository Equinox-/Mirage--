#include "ItemEqiups.h"

ItemEqiups::ItemEqiups(CurrentPlayers& currentPlayers) :
		mCurrentPlayers(currentPlayers) {

}

ItemEqiups::~ItemEqiups() {

}

void ItemEqiups::UpdateRaknet(Packet* packet, RakPeerInterface* peer) {
	switch (packet->data[0]) {
	case ID_EQIUP_ITEM: {
		int itemNumber = 0;
		int itemType = 0;
		int imageNumber = 0;

		BitStream bsIn(packet->data, packet->length, false);
		bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

		bsIn.Read(itemNumber);
		bsIn.Read(imageNumber);
		bsIn.Read(itemType);

		Player* player = mCurrentPlayers.GetPlayer(packet->guid);
		if (player->HasItem(itemNumber)) {
			//player->RemoveItem(itemNumber);
			int oldEqiuppedItem = player->EquipItem(
					(ItemType) itemType, itemNumber, imageNumber);

			if (oldEqiuppedItem > 0) {
				ItemBase* itemClass = ReadItem(oldEqiuppedItem, itemType);

				// Send the item the player used to have back to his inventory
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID) ID_PICKUP_ITEM);
				bsOut.Write(itemType);

				SendItemData(itemClass, bsOut);
				itemClass->SendData(bsOut);

				// Remove the old item bonuses
				player->ApplyItemBonuses(oldEqiuppedItem, itemType, packet,
						peer, false);

				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
						packet->guid, false);
			}

			// Let the client know they have a new item eqiuped :O
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID) ID_EQIUP_ITEM);

			bsOut.Write(itemType);
			bsOut.Write(itemNumber);
			bsOut.Write(imageNumber);

			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid,
					false);

			// Send them the item bonuses and apply them
			player->ApplyItemBonuses(itemNumber, itemType, packet, peer, true);
		}

		break;
	}

	case ID_UNEQIUP_ITEM: {
		int slot = 0;

		BitStream bsIn(packet->data, packet->length, false);
		bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

		Player* player = mCurrentPlayers.GetPlayer(packet->guid);
		bsIn.Read(slot);

		int itemNumber = player->UneqiupItem(slot);
		int itemType = 0;

		ItemBase* itemClass = ReadItem(itemNumber, itemType);

		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID) ID_PICKUP_ITEM);
		bsOut.Write(itemType);

		SendItemData(itemClass, bsOut);
		itemClass->SendData(bsOut);

		// Give them the item server side
		player->AddItem(itemNumber, 1);

		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid,
				false);

		// Send them the item bonuses and apply them
		player->ApplyItemBonuses(itemNumber, itemType, packet, peer, false);

		break;
	}
	}
}
