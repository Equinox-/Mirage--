#include "ChatBox.h"
ChatBox::ChatBox(Raknet& raknet) :
		mInput(25, 843, 150, 25, false, false, 255, 255, 255, 35, true), mChatSelected(
				false), mChatBoxLines(6), mRaknet(raknet), mInit(false) {

}

ChatBox::~ChatBox() {

}

void ChatBox::Init(int usernameLength) {
	mInput.Init();
	mFont.Load(15, true);
	mFont.SetColor(45, 133, 52);

	mSprite.Load("ChatBox.png");
	mCursor.Load("ChatLine.png");
	mSprite.SetPosition(10, 655);
	//mSprite.SetAlpha(215);

	mInput.SetMaxLength(45 - usernameLength);
	mInit = true;
}

void ChatBox::Update(float deltaTime) {
	if (mInit) {
		//mCursor.SetPosition(10 + (mInput.GetSize() * 10), 820);
		if (Input_IsKeyPressed(Keys::ENTER)
				|| Input_IsKeyPressed(Keys::NUM_ENTER)) {
			if (mChatSelected && mInput.GetSize() > 0) {
				if (mInput.GetString()[0] != '/') {
					// Send Chat Message
					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID) ID_CHATMESSAGE);
					bsOut.Write(mInput.GetString());
					mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED,
							0, mRaknet.mServerAddress, false);
				} else {
					Command(mInput.GetString());
				}

				mChatSelected = false;
				mInput.ClearText();
			} else {
				mChatSelected = true;
			}

			mInput.SetActive(mChatSelected);
		}

		mInput.Update(deltaTime);
	}
}

void ChatBox::Render() {
	if (mInit) {
		mSprite.Render();
		int position = 835;
		int count = 0;

		// Display the chat messages
		for (std::list<ChatMessages>::reverse_iterator iter =
				(mChatMessages).rbegin(); iter != (mChatMessages).rend();
				iter++) {
			if (count >= mChatBoxLines) {
				break;
			}

			mFont.Print(iter->mMessage, 55, position -= 18);
			++count;
		}

		mInput.Render();
		mCursor.Render();
	}
}

void ChatBox::UpdateRaknet() {
	if (mInit) {
		switch (mRaknet.mPacket->data[0]) {
		case ID_CHATMESSAGE: {
			RakString message;
			RakString name;

			// Read the chat message
			BitStream bsIn(mRaknet.mPacket->data, mRaknet.mPacket->length,
					false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(message);
			bsIn.Read(name);

			// Join the name and message together
			char temp[CHAR_MAX];
			sprintf(temp, "%s: %s", name.C_String(), message.C_String());

			// Check for multiline string
			if (message.GetLength() > 50) {

			}

			// Add our chat message to the list
			mChatMessages.push_back(ChatMessages(temp));
		}
		}
	}
}

void ChatBox::Command(char* string) {
	char* command = string;
	char* paramater1 = strchr(string, ' ') + 1;
	char* paramater2 = NULL;

	if (strcmp(command, "/setsprite") == 1) {
		if (IsInt(paramater1[0])) {
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID) ID_SET_SPRITE);
			bsOut.Write(atoi(paramater1));
			mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
					mRaknet.mServerAddress, false);
		} else {
			paramater2 = strchr(paramater1, ' ') + 1;

			for (int a = 0; paramater1[a] != '\0'; ++a) {
				if (paramater1[a] == ' ') {
					paramater1[a] = '\0';
					break;
				}
			}

			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID) ID_SET_SPRITE_NAME);
			bsOut.Write(paramater1);
			bsOut.Write(atoi(paramater2));
			mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
					mRaknet.mServerAddress, false);
		}
	}
}
