//====================================================================================================
// Filename:	CApplication.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "CApplication.h"
#include "../Misc/CIniFile.h"
#include "CLog.h"
#include <GL/freeglut.h>
#include "../Input/CDXInput.h"

//====================================================================================================
// Class Definitions
//====================================================================================================
static bool windowClosed = false;
static void glutClosing() {
	windowClosed = true;
}
CApplication::CApplication(void) :
		mWindow(0), mWinWidth(0), mWinHeight(0) {
	// Empty
}

//----------------------------------------------------------------------------------------------------

CApplication::~CApplication(void) {
	// Empty
}

//----------------------------------------------------------------------------------------------------

void CApplication::Initialize(const char* pAppName, int hInstance,
		int iWinWidth, int iWinHeight) {
	// Initialize error log
	CLog::Get()->Initialize();

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE,
			"[Application] Initializing: %s...", pAppName);
	glutInitWindowSize(iWinWidth, iWinHeight);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	// Cache the windows dimension
	mWinWidth = iWinWidth;
	mWinHeight = iWinHeight;

	mWindow = glutCreateWindow(pAppName);
	glutSetCursor(GLUT_CURSOR_NONE);

	// Do any additional initialization here
	OnInitialize();
	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[Application] %s initialized",
			pAppName);
}

//----------------------------------------------------------------------------------------------------

void CApplication::Terminate(void) {
	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[Application] Terminating...");

	// Do any additional termination tasks here
	OnTerminate();

	// Destroy the window
	glutDestroyWindow(mWindow);

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE,
			"[Application] Application terminated.");

	// Shutdown error log
	CLog::Get()->Terminate();
}

//----------------------------------------------------------------------------------------------------
void CApplication::Run(void) {
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
			GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutCloseFunc(glutClosing);
	while (!windowClosed) {
		OnMainLoop();
	}
	printf("Window closed!\n");
	//glutMainLoop();

	/*MSG msg;
	 memset(&msg, 0, sizeof(MSG));

	 // Start the message loop
	 while (WM_QUIT != msg.message) {
	 if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
	 TranslateMessage(&msg);
	 DispatchMessage(&msg);
	 } else {
	 // Run main loop
	 OnMainLoop();
	 }
	 }*/
}

//----------------------------------------------------------------------------------------------------

void CApplication::Quit(void) {
	//PostQuitMessage(0);
	windowClosed = true;
}

//----------------------------------------------------------------------------------------------------

void CApplication::OnInitialize(void) {
	// Empty
}

//----------------------------------------------------------------------------------------------------

void CApplication::OnTerminate(void) {
	// Empty
}

//----------------------------------------------------------------------------------------------------

void CApplication::OnMainLoop(void) {
	//if (KEYDOWN (VK_ESCAPE)) {
	//	Quit();
	//}
}
