#include "GamePlay.h"

GamePlay::GamePlay(Raknet& rak) :
		BaseState(rak), mConnectionTimer(0), mMap(rak, mPlayerInfo, mCharacter), mCharacter(
				mMap, mPlayerInfo, rak), mOtherPlayers(rak, mPlayerInfo), mScreenWidth(
				0), mScreenHeight(0), mCommands(rak, mPlayerInfo, mCharacter,
				mMap) {
	// Create the chat box
	mChatBox = new ChatBox(mRaknet);
	mHUD = new HUD(mPlayerInfo, mRaknet, mMap, mCharacter, *mChatBox,
			mOtherPlayers);
}

GamePlay::~GamePlay() {
	if (mChatBox) {
		delete mChatBox;
	}

	if (mHUD) {
		delete mHUD;
	}
}

void GamePlay::Load() {
	mCheckConnectionTime = (float) IniFile_GetInt("CONNECTION_TIMEOUT", 700);

	// Get Player Info
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID) ID_GET_PLAYER_INFO);
	mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
			mRaknet.mServerAddress, false);

	mMap.Init();
	mHUD->Init();

	mScreenWidth = IniFile_GetInt("WinWidth", 1600);
	mScreenHeight = IniFile_GetInt("WinHeight", 900);

	mOtherPlayers.Init();
}

void GamePlay::Unload() {
	if (mChatBox) {
		delete mChatBox;
		mChatBox = NULL;
	}

	mCharacter.Unload();
}

int GamePlay::Update(float deltaTime) {
	// check for state change
	GameState::Type nextState = GameState::Invalid;

	mConnectionTimer += deltaTime;

	if (mConnectionTimer > mCheckConnectionTime) {
//		ShowCursor(true);
//		MessageBoxA(NULL,"Disconnected from server",NULL,NULL);
//		ShowCursor(false);
		nextState = GameState::FrontEnd;
	}

	UpdateRaknet(deltaTime);

	SGE::Graphics_DebugUsageBegin("Map Update");
	mMap.Update(deltaTime);
	mCharacter.Update(deltaTime);
	mOtherPlayers.Update(deltaTime);
	SGE::Graphics_DebugUsageEnd();

	SGE::Graphics_DebugUsageBegin("HUD Update");
	mChatBox->Update(deltaTime);
	mHUD->Update(deltaTime);
	SGE::Graphics_DebugUsageEnd();

	// Check for item pickup
	if (Input_IsKeyPressed(Keys::ENTER)) {
		int item = mCharacter.OnItem();
		int itemIndex = mCharacter.GetOnItemIndex();

		if (item && mMap.GetItemActive(itemIndex)) {
			mChatBox->SetActive(false);
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID) ID_PICKUP_ITEM);
			bsOut.Write(item);
			bsOut.Write(mCharacter.GetOnItemIndex());
			bsOut.Write(mPlayerInfo.GetMap());
			mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
					mRaknet.mServerAddress, false);
		}
	}

	// Escape Key
	if (Input_IsKeyPressed(Keys::ESCAPE)) {
//		ShowCursor(true);
//		int result = MessageBoxA(NULL, "Are you sure you want to logout?", "Confirm", MB_YESNO);
//		ShowCursor(false);
		/* TODO if (1) //result == IDYES)
		 {
		 // Update our position before we leave
		 BitStream bsOut;
		 bsOut.Write((RakNet::MessageID) ID_UPDATE_POSITION);
		 bsOut.Write(mCharacter.GetPosition());
		 mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
		 mRaknet.mServerAddress, false);

		 nextState = GameState::Quit;
		 }*/
	}

	return nextState;
}

void GamePlay::Render() {
	SVector2 kTarget = mCharacter.GetPosition();
	const int kMapWidth = mMap.GetWidth() * 32;
	const int kMapHeight = mMap.GetHeight() * 32;

	SVector2 offset;
	offset.x = (mScreenWidth * 0.5f) - kTarget.x;
	offset.x = Clamp(offset.x, mScreenWidth - kMapWidth, 0.0f);
	offset.y = (mScreenHeight * 0.5f) - kTarget.y;
	offset.y = Clamp(offset.y, mScreenHeight - kMapHeight, 0.0f);

	SGE::Graphics_DebugUsageBegin("MapRender");
	mMap.Render(offset, mOtherPlayers);
	SGE::Graphics_DebugUsageEnd();

	SGE::Graphics_DebugUsageBegin("HUDRender");
	mChatBox->Render();
	mHUD->Render();
	SGE::Graphics_DebugUsageEnd();

#if _DEBUG
	char temp[CHAR_MAX];
	sprintf(temp,"Mouse X: %d", Input_GetMouseScreenX());
	Graphics_DebugText(temp, 1200,25, 0XFF0000);
	sprintf(temp,"Mouse Y: %d", Input_GetMouseScreenY());
	Graphics_DebugText(temp, 1200,50, 0XFF0000);
#endif
}

void GamePlay::UpdateRaknet(float deltaTime) {
	for (mRaknet.mPacket = mRaknet.mPeer->Receive(); mRaknet.mPacket;
			mRaknet.mPeer->DeallocatePacket(mRaknet.mPacket), mRaknet.mPacket =
					mRaknet.mPeer->Receive()) {
		mChatBox->UpdateRaknet();
		mMap.UpdateRaknet();
		mOtherPlayers.UpdateRaknet();
		mCommands.UpdateRaknet();
		mHUD->UpdateRaknet();

		switch (mRaknet.mPacket->data[0]) {
		case ID_STILLCONNECTED: {
			mConnectionTimer = 0;
			break;
		}

		case ID_GET_PLAYER_INFO: {
			RakNet::BitStream bsIn(mRaknet.mPacket->data,
					mRaknet.mPacket->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(mPlayerInfo);

			mChatBox->Init(mPlayerInfo.GetUsernameLength());
			mCharacter.Load();
			mHUD->InitEqiup();
			mOtherPlayers.GetPlayerActivePlayers();

			break;
		}
		}
	}
}
