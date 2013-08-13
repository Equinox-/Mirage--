//====================================================================================================
// Filename:	CDXInput.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "CDXInput.h"
#include "Input.h"
#include <GL/freeglut.h>
#include <unistd.h>
#include <string.h>
#include "../GLUtil/Math/Vector2f.h"

#include "../Core/CLog.h"
#include "../Misc/CIniFile.h"

#if __linux
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#endif

//====================================================================================================
// Statics
//====================================================================================================

CDXInput* CDXInput::s_Instance = NULL;

//====================================================================================================
// Class Definitions
//====================================================================================================

CDXInput* CDXInput::Get(void) {
	// If we do not yet have an instance created
	if (NULL == s_Instance) {
		// Create a new instance
		s_Instance = new CDXInput;
	}

	return s_Instance;
}

//----------------------------------------------------------------------------------------------------

CDXInput::CDXInput(void) :
		mMouseX(0), mMouseY(0), mScreenWidth(0), mScreenHeight(0), mInitialized(
				false) {
	memset(mCurrKeyBuffer, 0, kKeyBufferSize * sizeof(char));
	memset(mPrevKeyBuffer, 0, kKeyBufferSize * sizeof(char));
	mCurrMouseState = 0;
	mPrevMouseState = 0;
}

//----------------------------------------------------------------------------------------------------

void CDXInput::Initialize(int hWindow) {
	// Check if we have already initialized the system
	if (mInitialized) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING,
				"[Input] System already initialized.");
		return;
	}

	memcpy(mPrevKeyBuffer, mCurrKeyBuffer, kKeyBufferSize * sizeof(char));

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[Input] Initializing...");
	mScreenWidth = glutGet(GLUT_WINDOW_WIDTH);
	mScreenHeight = glutGet(GLUT_WINDOW_HEIGHT);
	mMouseX = 0;
	mMouseY = 0;

	mInitialized = true;

	glutSetWindow(hWindow);
	glutPassiveMotionFunc(cdxInput_mouseMotion);
	glutMotionFunc(cdxInput_mouseMotion);
	glutMouseFunc(cdxInput_mouseButton);
	glutKeyboardFunc(cdxInput_keyPress);
	glutSpecialFunc(cdxInput_keyPressSpecial);
	glutKeyboardUpFunc(cdxInput_keyRelease);
	glutSpecialUpFunc(cdxInput_keyReleaseSpecial);

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[Input] System initialized.");
}

void CDXInput::updateBuffers() {
	mPrevMouseState = mCurrMouseState;
}

void CDXInput::updateKeyModifiers() {
	int modifiers = glutGetModifiers();
	if (modifiers & GLUT_ACTIVE_SHIFT) {
		mCurrKeyBuffer[Keys::LSHIFT] = 0x80;
		mCurrKeyBuffer[Keys::RSHIFT] = 0x80;
	} else {
		mCurrKeyBuffer[Keys::LSHIFT] = 0;
		mCurrKeyBuffer[Keys::RSHIFT] = 0;
	}

	if (modifiers & GLUT_ACTIVE_CTRL) {
		mCurrKeyBuffer[Keys::LCONTROL] = 0x80;
		mCurrKeyBuffer[Keys::RCONTROL] = 0x80;
	} else {
		mCurrKeyBuffer[Keys::LCONTROL] = 0;
		mCurrKeyBuffer[Keys::RCONTROL] = 0;
	}

	if (modifiers & GLUT_ACTIVE_ALT) {
		mCurrKeyBuffer[Keys::LALT] = 0x80;
		mCurrKeyBuffer[Keys::RALT] = 0x80;
	} else {
		mCurrKeyBuffer[Keys::LALT] = 0;
		mCurrKeyBuffer[Keys::RALT] = 0;
	}
}

static void cdxInput_mouseMotion(int x, int y) {
	CDXInput *in = CDXInput::Get();
	in->updateBuffers();
	in->mMouseX = x;
	in->mMouseY = y;
}

static void cdxInput_mouseButton(int button, int state, int x, int y) {
	CDXInput *in = CDXInput::Get();
	in->updateBuffers();
	in->mMouseX = x;
	in->mMouseY = y;
	int mask = 0;
	switch (button) {
	case GLUT_LEFT_BUTTON:
		mask = Mouse::LBUTTON;
		break;
	case GLUT_RIGHT_BUTTON:
		mask = Mouse::RBUTTON;
		break;
	case GLUT_MIDDLE_BUTTON:
		mask = Mouse::MBUTTON;
		break;
	}
	if (state == GLUT_UP) {
		in->mCurrMouseState &= ~mask;
	} else {
		in->mCurrMouseState |= mask;
	}
}

static void cdxInput_keyPress(unsigned char c, int x, int y) {
	CDXInput *in = CDXInput::Get();
	in->updateKeyModifiers();
	in->mCurrKeyBuffer[tolower(c)] = 0x80;
}

static void cdxInput_keyRelease(unsigned char c, int x, int y) {
	CDXInput *in = CDXInput::Get();
	in->updateKeyModifiers();
	in->mCurrKeyBuffer[tolower(c)] = 0;
}

static void cdxInput_keyPressSpecial(int c, int x, int y) {
	CDXInput *in = CDXInput::Get();
	in->updateKeyModifiers();
	const int val = 0x80;
	switch (c) {
	case GLUT_KEY_F1:
		in->mCurrKeyBuffer[Keys::F1] = val;
		break;
	case GLUT_KEY_F2:
		in->mCurrKeyBuffer[Keys::F2] = val;
		break;
	case GLUT_KEY_F3:
		in->mCurrKeyBuffer[Keys::F3] = val;
		break;
	case GLUT_KEY_F4:
		in->mCurrKeyBuffer[Keys::F4] = val;
		break;
	case GLUT_KEY_F5:
		in->mCurrKeyBuffer[Keys::F5] = val;
		break;
	case GLUT_KEY_F6:
		in->mCurrKeyBuffer[Keys::F6] = val;
		break;
	case GLUT_KEY_F7:
		in->mCurrKeyBuffer[Keys::F7] = val;
		break;
	case GLUT_KEY_F8:
		in->mCurrKeyBuffer[Keys::F8] = val;
		break;
	case GLUT_KEY_F9:
		in->mCurrKeyBuffer[Keys::F9] = val;
		break;
	case GLUT_KEY_F10:
		in->mCurrKeyBuffer[Keys::F10] = val;
		break;
	case GLUT_KEY_F11:
		in->mCurrKeyBuffer[Keys::F11] = val;
		break;
	case GLUT_KEY_F12:
		in->mCurrKeyBuffer[Keys::F12] = val;
		break;
	case GLUT_KEY_LEFT:
		in->mCurrKeyBuffer[Keys::LEFT] = val;
		break;
	case GLUT_KEY_UP:
		in->mCurrKeyBuffer[Keys::UP] = val;
		break;
	case GLUT_KEY_RIGHT:
		in->mCurrKeyBuffer[Keys::RIGHT] = val;
		break;
	case GLUT_KEY_DOWN:
		in->mCurrKeyBuffer[Keys::DOWN] = val;
		break;
	case GLUT_KEY_PAGE_UP:
		in->mCurrKeyBuffer[Keys::PGUP] = val;
		break;
	case GLUT_KEY_PAGE_DOWN:
		in->mCurrKeyBuffer[Keys::PGDN] = val;
		break;
	case GLUT_KEY_HOME:
		in->mCurrKeyBuffer[Keys::HOME] = val;
		break;
	case GLUT_KEY_END:
		in->mCurrKeyBuffer[Keys::END] = val;
		break;
	case GLUT_KEY_INSERT:
		in->mCurrKeyBuffer[Keys::INS] = val;
		break;
	}
}

static void cdxInput_keyReleaseSpecial(int c, int x, int y) {
	CDXInput *in = CDXInput::Get();
	in->updateKeyModifiers();
	const int val = 0;
	switch (c) {
	case GLUT_KEY_F1:
		in->mCurrKeyBuffer[Keys::F1] = val;
		break;
	case GLUT_KEY_F2:
		in->mCurrKeyBuffer[Keys::F2] = val;
		break;
	case GLUT_KEY_F3:
		in->mCurrKeyBuffer[Keys::F3] = val;
		break;
	case GLUT_KEY_F4:
		in->mCurrKeyBuffer[Keys::F4] = val;
		break;
	case GLUT_KEY_F5:
		in->mCurrKeyBuffer[Keys::F5] = val;
		break;
	case GLUT_KEY_F6:
		in->mCurrKeyBuffer[Keys::F6] = val;
		break;
	case GLUT_KEY_F7:
		in->mCurrKeyBuffer[Keys::F7] = val;
		break;
	case GLUT_KEY_F8:
		in->mCurrKeyBuffer[Keys::F8] = val;
		break;
	case GLUT_KEY_F9:
		in->mCurrKeyBuffer[Keys::F9] = val;
		break;
	case GLUT_KEY_F10:
		in->mCurrKeyBuffer[Keys::F10] = val;
		break;
	case GLUT_KEY_F11:
		in->mCurrKeyBuffer[Keys::F11] = val;
		break;
	case GLUT_KEY_F12:
		in->mCurrKeyBuffer[Keys::F12] = val;
		break;
	case GLUT_KEY_LEFT:
		in->mCurrKeyBuffer[Keys::LEFT] = val;
		break;
	case GLUT_KEY_UP:
		in->mCurrKeyBuffer[Keys::UP] = val;
		break;
	case GLUT_KEY_RIGHT:
		in->mCurrKeyBuffer[Keys::RIGHT] = val;
		break;
	case GLUT_KEY_DOWN:
		in->mCurrKeyBuffer[Keys::DOWN] = val;
		break;
	case GLUT_KEY_PAGE_UP:
		in->mCurrKeyBuffer[Keys::PGUP] = val;
		break;
	case GLUT_KEY_PAGE_DOWN:
		in->mCurrKeyBuffer[Keys::PGDN] = val;
		break;
	case GLUT_KEY_HOME:
		in->mCurrKeyBuffer[Keys::HOME] = val;
		break;
	case GLUT_KEY_END:
		in->mCurrKeyBuffer[Keys::END] = val;
		break;
	case GLUT_KEY_INSERT:
		in->mCurrKeyBuffer[Keys::INS] = val;
		break;
	}
}
//----------------------------------------------------------------------------------------------------

void CDXInput::Terminate(void) {
	// Check if we have already terminated the system
	if (!mInitialized) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING,
				"[Input] System already terminated.");
		return;
	}

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[Input] Terminating...");

	// Set flag
	mInitialized = false;

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[Input] System terminated");
}

//----------------------------------------------------------------------------------------------------

void CDXInput::Update(void) {
	// Check if the system is initialized
	if (!mInitialized) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_ERROR,
				"[Input] Failed to update input devices. System not initialized.");
		return;
	}
}

//----------------------------------------------------------------------------------------------------

bool CDXInput::IsKeyDown(int iKey) const {
	return (mCurrKeyBuffer[iKey] & 0x80) ? true : false;
}

//----------------------------------------------------------------------------------------------------

bool CDXInput::IsKeyPressed(int iKey) const {
	bool bCurrState = (mCurrKeyBuffer[iKey] & 0x80) ? true : false;
	bool bPrevState = (mPrevKeyBuffer[iKey] & 0x80) ? true : false;

	return !bPrevState && bCurrState;
}

//----------------------------------------------------------------------------------------------------

bool CDXInput::IsMouseDown(int iMouse) const {
	return (mCurrMouseState & iMouse) ? true : false;
}

//----------------------------------------------------------------------------------------------------

bool CDXInput::IsMousePressed(int iMouse) const {
	bool bCurrState = (mCurrMouseState & iMouse) ? true : false;
	bool bPrevState = (mPrevMouseState & iMouse) ? true : false;
	return !bPrevState && bCurrState;
}

//----------------------------------------------------------------------------------------------------

int CDXInput::GetMouseScreenX(void) const {
	return mMouseX;
}

//----------------------------------------------------------------------------------------------------

int CDXInput::GetMouseScreenY(void) const {
	return mMouseY;
}

//----------------------------------------------------------------------------------------------------

bool CDXInput::IsMouseLeftEdge(void) const {
	return (mMouseX == 0);
}

//----------------------------------------------------------------------------------------------------

bool CDXInput::IsMouseRightEdge(void) const {
	return (mMouseX == mScreenWidth - 1);
}

//----------------------------------------------------------------------------------------------------

bool CDXInput::IsMouseTopEdge(void) const {
	return (mMouseY == 0);
}

//----------------------------------------------------------------------------------------------------

bool CDXInput::IsMouseBottomEdge(void) const {
	return (mMouseY == mScreenHeight - 1);
}
