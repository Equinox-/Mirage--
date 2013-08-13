#include "ItemHelper.h"
#include "ItemBase.h"

ItemBase* ReadItem(int itemNumber, int& itemType) {
	char temp[CHAR_MAX];
	sprintf(temp, "Items/%d.item", itemNumber);
	FILE* pFile = fopen(temp, "rb");

	int imageNumber = 0;
	ItemBase* item = NULL;

	if (pFile) {
		fscanf(pFile, "%d", &itemType);

		switch (itemType) {
		case potion: {
			Potion* pPotion = new Potion;
			int IgnoreVTable = 0;
			fread(&IgnoreVTable, 1, sizeof(int), pFile);
			fread(((char*) pPotion) + 4, 1, sizeof(Potion) - sizeof(int),
					pFile);
			item = pPotion;
			break;
		}

		case weapon: {
			Weapon* pWeapon = new Weapon;
			int IgnoreVTable = 0;
			fread(&IgnoreVTable, 1, sizeof(int), pFile);
			fread(((char*) pWeapon) + 4, 1, sizeof(Weapon) - sizeof(int),
					pFile);
			item = pWeapon;
			break;
		}

		case shield: {
			Shield* pShield = new Shield;
			int IgnoreVTable = 0;
			fread(&IgnoreVTable, 1, sizeof(int), pFile);
			fread(((char*) pShield) + 4, 1, sizeof(Shield) - sizeof(int),
					pFile);
			item = pShield;
			break;
		}

		case helmet: {
			Helmet* pHelmet = new Helmet;
			int IgnoreVTable = 0;
			fread(&IgnoreVTable, 1, sizeof(int), pFile);
			fread(((char*) pHelmet) + 4, 1, sizeof(Helmet) - sizeof(int),
					pFile);
			item = pHelmet;
			break;
		}

		case armor: {
			Armor* pArmor = new Armor;
			int IgnoreVTable = 0;
			fread(&IgnoreVTable, 1, sizeof(int), pFile);
			fread(((char*) pArmor) + 4, 1, sizeof(Armor) - sizeof(int), pFile);
			item = pArmor;
			break;
		}

		case boots: {
			Boots* pBoots = new Boots;
			int IgnoreVTable = 0;
			fread(&IgnoreVTable, 1, sizeof(int), pFile);
			fread(((char*) pBoots) + 4, 1, sizeof(Boots) - sizeof(int), pFile);
			item = pBoots;
			break;
		}

		case necklace: {
			Necklace* pNecklace = new Necklace;
			int IgnoreVTable = 0;
			fread(&IgnoreVTable, 1, sizeof(int), pFile);
			fread(((char*) pNecklace) + 4, 1, sizeof(Necklace) - sizeof(int),
					pFile);
			item = pNecklace;
			break;
		}

		case ring: {
			Ring* pRing = new Ring;
			int IgnoreVTable = 0;
			fread(&IgnoreVTable, 1, sizeof(int), pFile);
			fread(((char*) pRing) + 4, 1, sizeof(Ring) - sizeof(int), pFile);
			item = pRing;
			break;
		}
		}

		fclose(pFile);
	}

	return item;
}

void SendItemData(ItemBase* item, RakNet::BitStream& bsOut) {
	bsOut.Write(item->GetName());
	bsOut.Write(item->GetValue());
	bsOut.Write(item->GetQuest());
	bsOut.Write(item->GetImageNumber());
	bsOut.Write(item->GetDescription());
	bsOut.Write(item->GetItemNumber());
}
