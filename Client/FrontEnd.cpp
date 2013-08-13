#include "FrontEnd.h"

FrontEnd::FrontEnd(Raknet& rak) :
		mUsername(385, 275, 200, 15, false, true, 0, 0, 0, 5.0f, false, 12), mPassword(
				385, 325, 205, 15, true, true, 0, 0, 0, 5.0f, false), mEmail(
				385, 375, 200, 15, false, true, 0, 0, 0, 5.0f, false), mCurrentMenu(
				Main), BaseState(rak), mNextState(GameState::Invalid), mTimeOutTimer(
				0), mAcceptButtonHit(false), mSoundOn(true) {

}

FrontEnd::~FrontEnd() {
	mBackground.Unload();
	mMenu.Unload();
	mLogin.Unload();
	mRegister.Unload();
	mCredits.Unload();
	mExit.Unload();
}

void FrontEnd::Load() {
	mMenuTimeout = (float) IniFile_GetInt("MENU_TIMEOUT", 5);
	mSoundOn = IniFile_GetBool("Sound", true);

	// Connect to the server if we can
	mRaknet.Connect();

	mUsername.Init();
	mPassword.Init();
	mEmail.Init();

	mBackground.Load("GUI/FrontEnd/background.png");
	mBackground.SetPosition(250, 200);

	mMenu.Add("GUI/FrontEnd/main.png");
	mMenu.Add("GUI/FrontEnd/credits.png");
	mMenu.Add("GUI/FrontEnd/login.png");
	mMenu.Add("GUI/FrontEnd/register.png");
	mMenu.SetPosition(287, 212);

	mLogin.Load("GUI/FrontEnd/buttons/login_click.png",
			"GUI/FrontEnd/buttons/login_norm.png", 0);
	mRegister.Load("GUI/FrontEnd/buttons/register_click.png",
			"GUI/FrontEnd/buttons/register_norm.png", 0);
	mCredits.Load("GUI/FrontEnd/buttons/credits_click.png",
			"GUI/FrontEnd/buttons/credits_norm.png", 0);
	mExit.Load("GUI/FrontEnd/buttons/exit_click.png",
			"GUI/FrontEnd/buttons/exit_norm.png", 0);

	mSound.Load("Main.wav");

	if (mSoundOn) {
		mSound.Play(true);
	}

	mLogin.SetPosition(314, 487);
	mRegister.SetPosition(414, 487);
	mCredits.SetPosition(514, 487);
	mExit.SetPosition(614, 487);

	mFont.Load(18, true, false);
	mFont.SetColor(255, 255, 255);

	mAccept.Load("GUI/FrontEnd/buttons/accept.png",
			"GUI/FrontEnd/buttons/accept.png", 0);
	mAccept.SetPosition(485, 425);
}

void FrontEnd::Unload() {

}

int FrontEnd::Update(float deltaTime) {
	UpdateRaknet();

	// Check for server timeouts on register login etc
	if (mAcceptButtonHit) {
		++mTimeOutTimer;

		if (mTimeOutTimer > mMenuTimeout) {
			//ShowCursor(true);
			//MessageBoxA(NULL, "Error, Server must be down", NULL, NULL);
			//ShowCursor(false);
			mTimeOutTimer = 0;
			mAcceptButtonHit = false;
		}
	}

	// Update Buttons
	mBackground.Update(deltaTime);
	mMenu.Update(deltaTime);
	mLogin.Update(deltaTime);
	mRegister.Update(deltaTime);
	mCredits.Update(deltaTime);
	mExit.Update(deltaTime);
	mAccept.Update(deltaTime);

	// Update Input Boxes
	mUsername.Update(deltaTime);
	mPassword.Update(deltaTime);
	mEmail.Update(deltaTime);

	// Check for menu button pressis
	if (mLogin.IsPressed()) {
		mCurrentMenu = Login;
		mSound.Unload();
	} else if (mRegister.IsPressed()) {
		mCurrentMenu = Register;
	} else if (mCredits.IsPressed()) {
		mCurrentMenu = Credits;
	} else if (mExit.IsPressed()) {
		mNextState = GameState::Quit;
		mSound.Unload();
	}

	// Escape Key
	if (Input_IsKeyPressed(Keys::ESCAPE)) {
		mNextState = GameState::Quit;
		mSound.Unload();
	}

//#ifdef _AUTOLOG
	BitStream bsOut; //TODO
	bsOut.Write((RakNet::MessageID) ID_LOGIN);
	bsOut.Write("test2");
	bsOut.Write("test");
	mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
			mRaknet.mServerAddress, false);
	mSound.Unload();
//#endif

	return mNextState;
}

void FrontEnd::Render() {
	mBackground.Render();
	mMenu.Render();
	mLogin.Render();
	mRegister.Render();
	mCredits.Render();
	mExit.Render();

	// Setup the right menu item
	mMenu.SetCurrentFrame(mCurrentMenu);
	switch (mCurrentMenu) {
	case Login: {
		mUsername.Render();
		mPassword.Render();
		mAccept.Render();
		mFont.Print("Username: ", 385, 250);
		mFont.Print("Password: ", 385, 305);

		if (mAccept.IsPressed()) {
			if (mUsername.GetSize() > 0 && mPassword.GetSize() > 0) {
				// Send Login info to server
				BitStream bsOut;
				bsOut.Write((RakNet::MessageID) ID_LOGIN);
				bsOut.Write(mUsername.GetString());
				bsOut.Write(mPassword.GetString());
				mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
						mRaknet.mServerAddress, false);
				mAcceptButtonHit = true;
			} else {
				//ShowCursor(true);
				//MessageBoxA(NULL,"Make sure all the input boxes are fulled out.", NULL, NULL);
				//ShowCursor(false);
			}
		}
		break;
	}
	case Register: {
		mUsername.Render();
		mPassword.Render();
		mEmail.Render();
		mAccept.Render();
		mFont.Print("Username: ", 385, 250);
		mFont.Print("Password: ", 385, 305);
		mFont.Print("Email: ", 385, 355);

		if (mAccept.IsPressed()) {
			if (mUsername.GetSize() > 0 && mEmail.GetSize() > 0
					&& mPassword.GetSize() > 0) {
				// Send Registration info to server
				BitStream bsOut;
				bsOut.Write((RakNet::MessageID) ID_NEW_ACCOUNT);
				bsOut.Write(mUsername.GetString());
				bsOut.Write(mPassword.GetString());
				bsOut.Write(mEmail.GetString());
				mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
						mRaknet.mServerAddress, false);
				mAcceptButtonHit = true;
			} else {
				//	ShowCursor(true);
				//	MessageBoxA(NULL,"Make sure all the input boxes are fulled out.", NULL, NULL);
				//	ShowCursor(false);
			}
		}

		break;
	}

	case Main: {
		mFont.Print(mNews.C_String(), 320, 255);
		break;
	}
	}
}

void FrontEnd::UpdateRaknet() {
	for (mRaknet.mPacket = mRaknet.mPeer->Receive(); mRaknet.mPacket;
			mRaknet.mPeer->DeallocatePacket(mRaknet.mPacket), mRaknet.mPacket =
					mRaknet.mPeer->Receive()) {
		switch (mRaknet.mPacket->data[0]) {
		case ID_CONNECTION_REQUEST_ACCEPTED: {
			// Ask for the news
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID) ID_GET_NEWS);
			mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
					mRaknet.mPacket->systemAddress, false);
			mRaknet.mServerAddress = mRaknet.mPacket->systemAddress;

			break;
		}

		case ID_GET_NEWS: {
			RakNet::BitStream bsIn(mRaknet.mPacket->data,
					mRaknet.mPacket->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(mNews);

			// Split string into readable news
			int stringLength = mNews.GetLength();

			int count = 0;
			for (int a = 0; a < stringLength; ++a) {
				// If we need to split the string wait until we found a space to split it at
				if (count > 45) {
					if (mNews.sharedString->c_str[a] == ' ') {
						mNews.sharedString->c_str[a] = '\n';
						count = 0;
					}
				}

				++count;
			}

			break;
		}

		case ID_NEW_ACCOUNT: {
			fprintf(stderr, "Account Created.\n");
//				ShowCursor(true);
//				MessageBoxA(NULL, "Account Created", "Success", NULL);
//				ShowCursor(false);
			mAcceptButtonHit = false;
			mTimeOutTimer = 0;
			break;
		}

		case ID_ACCOUNT_TAKEN: {
			mAcceptButtonHit = false;
			mTimeOutTimer = 0;
			fprintf(stderr, "That Account name is already taken. Try Again.\n");
//				ShowCursor(true);
//				MessageBoxA(NULL, "That Account name is already taken. Try Again.", NULL, NULL);
//				ShowCursor(false);
			break;
		}

		case ID_LOGIN: {
			// Successfully logged in
			mAcceptButtonHit = false;
			mTimeOutTimer = 0;
			mNextState = GameState::Gameplay;
			break;
		}

		case ID_USER_BANNED: {
			fprintf(stderr, "You are currently banned from the server.\n");
//				MessageBoxA(NULL, "You are currently banned from the server.", NULL, NULL);
			break;
		}

		case ID_LOGIN_WRONG_PASS: {
			mAcceptButtonHit = false;
			mTimeOutTimer = 0;
			fprintf(stderr, "Wrong Password. Or Account already logged in.\n");
//				ShowCursor(true);
//				MessageBoxA(NULL,"Wrong Password. Or Account already logged in.", NULL, NULL);
//				ShowCursor(false);
			break;
		}
		}
	}
}
