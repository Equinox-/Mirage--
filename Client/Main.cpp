//====================================================================================================
// Includes
//====================================================================================================

#include <SGE.h>
#include <GL/glut.h>
#include "BaseState.h"
#include "GameStates.h"
#include "FrontEnd.h"
#include "GamePlay.h"
#include "Raknet.h"

using namespace SGE;

//====================================================================================================
// Globals
//====================================================================================================

bool gQuit = false;
BaseState* gCurrentState = NULL;
GameState::Type gNextState = GameState::FrontEnd;
SGE_Cursor gCursor;
Raknet gRaknet;

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================
void SGE_Initialize() {
	srand((unsigned int) time(NULL));
	gCursor.Load("cursor.png");
}

void SGE_Terminate() {

}

bool SGE_Update(float deltaTime) {
	// Update Cursor Position
	gCursor.Update(deltaTime);
	// check if we need to change state
	if (gNextState != GameState::Invalid) {
		if (gCurrentState != NULL) {
			gCurrentState->Unload();
			delete gCurrentState;
		}
		//switch states
		switch (gNextState) {
		case GameState::FrontEnd:
			gCurrentState = new FrontEnd(gRaknet);
			break;
		case GameState::Gameplay:
			gCurrentState = new GamePlay(gRaknet);
			break;
		}

		// enter the new state
		if (gCurrentState != NULL) {
			gCurrentState->Load();
		}
	}
	// update current state
	gNextState = static_cast<GameState::Type>(gCurrentState->Update(deltaTime));

	if (gNextState == GameState::Quit) {
		gRaknet.Disconnect();
		gQuit = true;
	}

	return gQuit;
}

void SGE_Render() {
	if (gCurrentState != NULL) {
		gCurrentState->Render();
	}
	gCursor.Render();
}
