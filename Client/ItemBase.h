#ifndef _ITEMBASE_H
#define _ITEMBASE_H

#include <limits.h>
#include <stdio.h>
#include <string>
#include "Raknet.h"

enum ItemType
{
	none = -1
	,weapon
	,shield
	,helmet
	,armor
	,boots
	,necklace
	,ring
	,ring2
	,potion
	,maxItems
};

class PlayerInfo;
class ItemBase
{
public:
	ItemBase():mType(none), mValue(1),mQuestItem(false), mImageNumber(1), mItemNumber(0) {};

    virtual ~ItemBase() {};
    virtual void Init() = 0;
	virtual void GetData(BitStream& bsIn) = 0;
	virtual void UseItem(PlayerInfo& playerInfo, Raknet& raknet) = 0;

	void RemoveItem(Raknet& raknet)
	{
		// Tell the server we used our item
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_REMOVE_ITEM);
		bsOut.Write(mItemNumber);
		raknet.mPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,raknet.mServerAddress,false);
	}

	void SetName(char* name)					{ strcpy(mItemName, name); }
	void SetValue(int value)					{ mValue = value; }
	void SetQuest(bool questItem)				{ mQuestItem = questItem; }
	void SetImageNumber(int number)				{ mImageNumber = number; }
	void SetType(ItemType type)					{ mType = type; }
	void SetDescription(char* description)		{ strcpy(mDescription, description); }
	void SetItemNumber(int number)				{ mItemNumber = number; }

	int GetImageNumber()						{ return mImageNumber; }
	char* GetDescription()						{ return mDescription; }
	char* GetName()								{ return mItemName; }
	int GetValue()								{ return mValue; }
	bool GetQuest()								{ return mQuestItem; }
	int GetItemNumber()							{ return mItemNumber; }
	ItemType GetType()							{ return mType; }

	
	

protected:
   ItemType mType;
   char mItemName[CHAR_MAX];
   int mValue;
   bool mQuestItem;
   int mImageNumber;
   char mDescription[1000];
   ItemBase( const ItemBase& other );
   int mItemNumber;
};

#endif