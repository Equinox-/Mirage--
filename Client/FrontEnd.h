#pragma once

#ifndef INCLUDED_FRONTEND_H
#define INCLUDED_FRONTEND_H

//====================================================================================================
// Includes
//====================================================================================================
#include <SGE.h>
using namespace SGE;

#include "BaseState.h"
#include "GameStates.h"
#include "InputBox.h"
#include "Raknet.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" 
#include "PlayerInfo.h"

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================


enum Menu
{
	Main,
	Credits,
	Login,
	Register,
	Exit
};

class FrontEnd : public BaseState
{
public:
	FrontEnd(Raknet& rak);
	~FrontEnd(void);

	// Implements base state
	virtual void Load();
	virtual void Unload();
	virtual int Update(float deltaTime);
	virtual void Render();

	void UpdateRaknet();
	
private:
	SGE_Sprite mBackground;
	SGE_Sprite mMenu;
	
	SGE_Button mLogin;
	SGE_Button mRegister;
	SGE_Button mCredits;
	SGE_Button mExit;
	SGE_Button mAccept;

	SGE_Font mFont;

	InputBox mUsername;
	InputBox mPassword;
	InputBox mEmail;

	Menu mCurrentMenu;
	RakString mNews;

	float mTimeOutTimer;
	float mMenuTimeout;

	bool mAcceptButtonHit;
	GameState::Type mNextState;
	SGE_Sound mSound;
	bool mSoundOn;
};

#endif
