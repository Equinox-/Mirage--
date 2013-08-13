#ifndef _CHATBOX_H
#define _CHATBOX_H

#include <SGE.h>
#include "InputBox.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" 
#include "Raknet.h"
#include "HelperFunctions.h"
#include <list>

using namespace RakNet;
using namespace SGE;

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

struct ChatMessages
{
	ChatMessages(char* message) { strcpy(mMessage, message); }
	char mMessage[CHAR_MAX];
};

class ChatBox
{
public:
	ChatBox(Raknet& raknet);
	~ChatBox();

	void Init(int usernameLength);
	void Update(float deltaTime);
	void UpdateRaknet();
	void Render();
	void SetActive(bool active)						{ mInput.SetActive(active); }
	void Command(char* string);
	bool& IsChatting()								{ return mChatSelected; }

private:
	SGE_Font mFont;
	SGE_Sprite mSprite;
	SGE_Sprite mCursor;
	InputBox mInput;
	
	bool mChatSelected;

	int mChatBoxLines;
	Raknet& mRaknet;

	bool mInit;

	std::list<ChatMessages> mChatMessages;
};

#endif