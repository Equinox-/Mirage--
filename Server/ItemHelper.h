#ifndef _ITEMHELPER_H
#define _ITEMHELPER_H

#include "ItemBase.h"
#include "Potion.h"
#include "Weapon.h"
#include "Shield.h"
#include "Helmet.h"
#include "Armor.h"
#include "Boots.h"
#include "Necklace.h"
#include "Ring.h"

ItemBase* ReadItem(int itemNumber, int& itemType);
void SendItemData(ItemBase* item, RakNet::BitStream& bsOut);
#endif