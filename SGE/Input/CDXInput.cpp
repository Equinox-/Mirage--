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
	glutPassiveMotionFunc(CDXInput::cdxInput_mouseMotion);
	glutMotionFunc(CDXInput::cdxInput_mouseMotion);
	glutMouseFunc(CDXInput::cdxInput_mouseButton);
	glutKeyboardFunc(CDXInput::cdxInput_keyPress);
	glutSpecialFunc(CDXInput::cdxInput_keyPressSpecial);
	glutKeyboardUpFunc(CDXInput::cdxInput_keyRelease);
	glutSpecialUpFunc(CDXInput::cdxInput_keyReleaseSpecial);

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[Input] System initialized.");
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

int CDXInput::cdxInput_glutKeyToSGE(int key) {
	switch (key) {
	case GLUT_KEY_F1:
		return Keys::F1;
		break;
	case GLUT_KEY_F2:
		return Keys::F2;
		break;
	case GLUT_KEY_F3:
		return Keys::F3;
		break;
	case GLUT_KEY_F4:
		return Keys::F4;
		break;
	case GLUT_KEY_F5:
		return Keys::F5;
		break;
	case GLUT_KEY_F6:
		return Keys::F6;
		break;
	case GLUT_KEY_F7:
		return Keys::F7;
		break;
	case GLUT_KEY_F8:
		return Keys::F8;
		break;
	case GLUT_KEY_F9:
		return Keys::F9;
		break;
	case GLUT_KEY_F10:
		return Keys::F10;
		break;
	case GLUT_KEY_F11:
		return Keys::F11;
		break;
	case GLUT_KEY_F12:
		return Keys::F12;
		break;
	case GLUT_KEY_LEFT:
		return Keys::LEFT;
		break;
	case GLUT_KEY_UP:
		return Keys::UP;
		break;
	case GLUT_KEY_RIGHT:
		return Keys::RIGHT;
		break;
	case GLUT_KEY_DOWN:
		return Keys::DOWN;
		break;
	case GLUT_KEY_PAGE_UP:
		return Keys::PGUP;
		break;
	case GLUT_KEY_PAGE_DOWN:
		return Keys::PGDN;
		break;
	case GLUT_KEY_HOME:
		return Keys::HOME;
		break;
	case GLUT_KEY_END:
		return Keys::END;
		break;
	case GLUT_KEY_INSERT:
		return Keys::INS;
		break;
	}
	return -1;
}

int CDXInput::cdxInput_characterToSGE(char c) {
	c = tolower(c);
	if (c >= 'a' && c <= 'z') {
		return c;
	}
	if (c >= '0' && c <= '9') {
		return c;
	}
	switch (c) {
	case '\n':
	case '\b':
	case '\t':
		return c;
	case '-':
	case '_':
		return '-';
	case '=':
	case '+':
		return '=';
	case '[':
	case '{':
		return '[';
	case ']':
	case '}':
		return ']';
	case ':':
	case ';':
		return ';';
	case '"':
	case '\'':
		return '\'';
	case '<':
	case ',':
		return ',';
	case '>':
	case '.':
		return '.';
	case '?':
	case '/':
		return '/';
	case '~':
	case '`':
		return '`';
	case '!':
		return '1';
	case '@':
		return '2';
	case '#':
		return '3';
	case '$':
		return '4';
	case '%':
		return '5';
	case '^':
		return '6';
	case '&':
		return '7';
	case '*':
		return '8';
	case '(':
		return '9';
	case ')':
		return '0';
	}
	return -1;
}

void CDXInput::cdxInput_mouseMotion(int x, int y) {
	CDXInput *in = CDXInput::Get();
	in->mMouseX = x;
	in->mMouseY = y;
}

void CDXInput::cdxInput_mouseButton(int button, int state, int x, int y) {
	CDXInput *in = CDXInput::Get();
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

void CDXInput::cdxInput_keyPress(unsigned char c, int x, int y) {
	CDXInput *in = CDXInput::Get();
	in->updateKeyModifiers();
	int keyID = cdxInput_characterToSGE(c);
	if (keyID > 0) {
		in->mCurrKeyBuffer[keyID] = 0x80;
	}
}

void CDXInput::cdxInput_keyRelease(unsigned char c, int x, int y) {
	CDXInput *in = CDXInput::Get();
	in->updateKeyModifiers();
	int keyID = cdxInput_characterToSGE(c);
	if (keyID > 0) {
		in->mCurrKeyBuffer[keyID] = 0;
	}
}

void CDXInput::cdxInput_keyPressSpecial(int c, int x, int y) {
	CDXInput *in = CDXInput::Get();
	in->updateKeyModifiers();
	int key = cdxInput_glutKeyToSGE(c);
	if (key >= 0) {
		in->mCurrKeyBuffer[key] = 0x80;
	}
}

void CDXInput::cdxInput_keyReleaseSpecial(int c, int x, int y) {
	CDXInput *in = CDXInput::Get();
	in->updateKeyModifiers();
	int key = cdxInput_glutKeyToSGE(c);
	if (key >= 0) {
		in->mCurrKeyBuffer[key] = 0;
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

	// Update buffers
	memcpy(mPrevKeyBuffer, mCurrKeyBuffer, kKeyBufferSize * sizeof(char));
	mPrevMouseState = mCurrMouseState;
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
