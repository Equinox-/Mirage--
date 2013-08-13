#include "Player.h"
#include <BitStream.h>

Player::Player(const char* username, const char* email, const char* password,
		const char* ip) :
		mGUID(0), mBanned(false), mPosition(64.0f, 64.0f), mLevel(0), mMap(1), mSpriteNumber(
				1), mDirection(down), mCurrentHealth(100), mTotalHealth(100), mCurrentMana(
				100), mTotalMana(100), mCurrentExp(0), mTotalExp(100), mStr(5), mDex(
				5), mInt(5), mAttack(5), mDefence(5), mCritChance(1), mAccessLevel(
				0), mGold(0) {
	strcpy(mUsername, username);
	strcpy(mPassword, password);
	strcpy(mEmail, email);
	strcpy(mIP, ip);

	// Clear Items
	for (int a = 0; a < 50; ++a) {
		mItems[a].first = 0;
		mItems[a].second = 0;
	}

	for (int a = 0; a < 9; ++a) {
		mEqiupment[a].first = 0;
		mEqiupment[a].second = 0;
	}
}

Player::Player(const char* username) :
		mGUID(0), mBanned(false), mPosition(0.0f, 0.0f), mLevel(0), mMap(1), mSpriteNumber(
				3), mAccessLevel(0) {
	strcpy(mUsername, username);

	mIP[0] = '\0';
	mEmail[0] = '\0';
	mPassword[0] = '\0';
}

Player::~Player() {

}

void Player::Load() {
	char temp[CHAR_MAX];
	sprintf(temp, "Accounts/%s.acc", mUsername);
	FILE* pFile = fopen(temp, "rb");

	if (pFile) {
		fread(this, 1, sizeof(*this), pFile);
		fclose(pFile);
	}
}

void Player::Save() {
	if (this->GetUsername()) {
		char temp[CHAR_MAX];
		sprintf(temp, "Accounts/%s.acc", mUsername);
		FILE* pFile = fopen(temp, "wb");
		fwrite(this, 1, sizeof(*this), pFile);

		fclose(pFile);
	}
}

bool Player::Create() {
	bool success = false;
	char temp[CHAR_MAX];
	sprintf(temp, "Accounts/%s.acc", mUsername);

	FILE* pFile = fopen(temp, "r");

	// If the file exists that user already has a account
	if (!pFile) {
		// Account doesnt exist create file
		Save();
		success = true;
	} else {
		fclose(pFile);
		success = false;
	}

	return success;
}

void Player::AddItem(int item, int number) {
	// Find the first availible item slot
	for (int a = 0; a < 50; ++a) {
		if (mItems[a].first == 0) {
			mItems[a].first = item;
			mItems[a].second = number;
			return;
		}
	}
}
void Player::RemoveItem(int itemNumber) {
	// Find the first availible item slot
	for (int a = 0; a < 50; ++a) {
		if (mItems[a].first == itemNumber) {
			mItems[a].first = 0;
			mItems[a].second = 0;
			return;
		}
	}
}

bool Player::HasItem(int itemNumber) {
	for (int a = 0; a < 50; ++a) {
		if (mItems[a].first == itemNumber) {
			return true;
		}
	}

	return false;
}

int Player::EquipItem(ItemType slot, int itemNumber, int imageNumber) {
	int oldItem = 0;

	if (slot != ring) {
		// Give them the currently eqiupped weapon back
		if (mEqiupment[slot].first > 0) {
			AddItem(mEqiupment[slot].first, 1);
			oldItem = mEqiupment[slot].first;
		}

		mEqiupment[slot].first = itemNumber;
		mEqiupment[slot].second = imageNumber;
	} else {
		// If the second ring is full swap out the first. If they are both full also swap out the first
		if (mEqiupment[ring2].first > 0) {
			AddItem(mEqiupment[ring].first, 1);
			oldItem = mEqiupment[ring].first;
			mEqiupment[ring].first = itemNumber;
			mEqiupment[ring].second = imageNumber;
		} else // Second slot is not full fill that one
		{
			AddItem(mEqiupment[ring2].first, 1);
			oldItem = mEqiupment[ring2].first;
			mEqiupment[ring2].first = itemNumber;
			mEqiupment[ring2].second = imageNumber;
		}
	}

	return oldItem;
}

void Player::GetItems(Packet* packet, RakPeerInterface* peer) {
	// Find the first availible item slot
	for (int a = 0; a < 50; ++a) {
		if (mItems[a].first != 0) {
			int itemType = 0;
			ItemBase* item = ReadItem(mItems[a].first, itemType);

			// Send the player the info that they can pick it up :O
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID) ID_PICKUP_ITEM);
			bsOut.Write(itemType);

			bsOut.Write(item->GetName());
			bsOut.Write(item->GetValue());
			bsOut.Write(item->GetQuest());
			bsOut.Write(item->GetImageNumber());
			bsOut.Write(item->GetDescription());
			bsOut.Write(item->GetItemNumber());

			item->SendData(bsOut);

			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid,
					false);
		}
	}
}

void Player::AddHealth(int health) {
	mCurrentHealth += health;

	if (mCurrentHealth > mTotalHealth) {
		mCurrentHealth = mTotalHealth;
	}
}

void Player::AddMana(int mana) {
	mCurrentMana += mana;

	if (mCurrentMana > mTotalMana) {
		mCurrentMana = mTotalMana;
	}
}

int Player::UneqiupItem(int slot) {
	int itemNumber = mEqiupment[slot].first;
	mEqiupment[slot].first = 0;
	mEqiupment[slot].second = 0;
	return itemNumber;
}

void Player::ApplyItemBonuses(int itemNumber, int type, Packet* packet,
		RakPeerInterface* peer, bool eqiup) {
	int slot = type;
	ItemBase* itemBase = ReadItem(itemNumber, slot);

	int str = 0;
	int dex = 0;
	int intStat = 0;
	int atk = 0;
	int defence = 0;
	int critChance = 0;
	int health = 0;
	int mana = 0;

	switch (type) {
	case weapon: {
		Weapon* weapon = reinterpret_cast<Weapon*>(itemBase);
		str = weapon->GetStrBonus();
		dex = weapon->GetDexBonus();
		intStat = weapon->GetIntBonus();
		atk = weapon->GetAtkBonus();
		defence = weapon->GetDefBonus();
		critChance = weapon->GetCritBonus();
		health = weapon->GetHealthBonus();
		mana = weapon->GetManaBonus();
		break;
	}

	case armor: {
		Armor* armor = reinterpret_cast<Armor*>(itemBase);
		str = armor->GetStrBonus();
		dex = armor->GetDexBonus();
		intStat = armor->GetIntBonus();
		atk = armor->GetAtkBonus();
		defence = armor->GetDefBonus();
		critChance = armor->GetCritBonus();
		health = armor->GetHealthBonus();
		mana = armor->GetManaBonus();
		break;
	}

	case boots: {
		Boots* boots = reinterpret_cast<Boots*>(itemBase);
		str = boots->GetStrBonus();
		dex = boots->GetDexBonus();
		intStat = boots->GetIntBonus();
		atk = boots->GetAtkBonus();
		defence = boots->GetDefBonus();
		critChance = boots->GetCritBonus();
		health = boots->GetHealthBonus();
		mana = boots->GetManaBonus();
		break;
	}

	case helmet: {
		Helmet* helmet = reinterpret_cast<Helmet*>(itemBase);
		str = helmet->GetStrBonus();
		dex = helmet->GetDexBonus();
		intStat = helmet->GetIntBonus();
		atk = helmet->GetAtkBonus();
		defence = helmet->GetDefBonus();
		critChance = helmet->GetCritBonus();
		health = helmet->GetHealthBonus();
		mana = helmet->GetManaBonus();
		break;
	}

	case necklace: {
		Necklace* necklace = reinterpret_cast<Necklace*>(itemBase);
		str = necklace->GetStrBonus();
		dex = necklace->GetDexBonus();
		intStat = necklace->GetIntBonus();
		atk = necklace->GetAtkBonus();
		defence = necklace->GetDefBonus();
		critChance = necklace->GetCritBonus();
		health = necklace->GetHealthBonus();
		mana = necklace->GetManaBonus();
		break;
	}

	case ring: {
		Ring* ring = reinterpret_cast<Ring*>(itemBase);
		str = ring->GetStrBonus();
		dex = ring->GetDexBonus();
		intStat = ring->GetIntBonus();
		atk = ring->GetAtkBonus();
		defence = ring->GetDefBonus();
		critChance = ring->GetCritBonus();
		health = ring->GetHealthBonus();
		mana = ring->GetManaBonus();
		break;
	}

	case shield: {
		Shield* shield = reinterpret_cast<Shield*>(itemBase);
		str = shield->GetStrBonus();
		dex = shield->GetDexBonus();
		intStat = shield->GetIntBonus();
		atk = shield->GetAtkBonus();
		defence = shield->GetDefBonus();
		critChance = shield->GetCritBonus();
		health = shield->GetHealthBonus();
		mana = shield->GetManaBonus();
		break;
	}
	}

	// Negate the value
	if (!eqiup) {
		str *= -1;
		dex *= -1;
		intStat *= -1;
		atk *= -1;
		defence *= -1;
		critChance *= -1;
		health *= -1;
		mana *= -1;
	}

	mStr += str;
	mDex += dex;
	mInt += intStat;
	mAttack += atk;
	mDefence += defence;
	mCritChance += critChance;
	mTotalHealth += health;
	mTotalMana += mana;

	// Send the client there bonuses

	if (mStr || mDex || mInt || mAttack || mDefence || mCritChance || health
			|| mana) {
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID) ID_ITEM_BONUS);

		bsOut.Write(str);
		bsOut.Write(dex);
		bsOut.Write(intStat);
		bsOut.Write(atk);
		bsOut.Write(defence);
		bsOut.Write(critChance);
		bsOut.Write(health);
		bsOut.Write(mana);
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid,
				false);
	}
}
