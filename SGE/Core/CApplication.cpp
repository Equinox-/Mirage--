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

	/*if (CIniFile::Get()->GetBool("ExitButton", true)) {
	 // Create a new window
	 mWindow = CreateWindowExA(NULL,					   // Extended style
	 pAppName,				   // Class name
	 pAppName,				   // Window name
	 WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_EX_TOPMOST,	// Style
	 0, 0,					   // Position
	 mWinWidth, mWinHeight,	// Dimensions
	 NULL,					   // Handle to parent window
	 NULL,					   // Handle to menu
	 hInstance,				  // Handle to instance
	 NULL						// Param
	 );*/
	mWindow = glutCreateWindow(pAppName);
	/*} else {
	 // Create a new window
	 mWindow = CreateWindowExA(NULL,					   // Extended style
	 pAppName,				   // Class name
	 pAppName,				   // Window name
	 WS_VISIBLE | WS_CAPTION | WS_EX_TOPMOST,			  // Style
	 0, 0,					   // Position
	 mWinWidth, mWinHeight,	// Dimensions
	 NULL,					   // Handle to parent window
	 NULL,					   // Handle to menu
	 hInstance,				  // Handle to instance
	 NULL						// Param
	 );
	 }*/

	/*RECT clientRect;
	 clientRect.left = 0;
	 clientRect.top = 0;
	 clientRect.right = mWinWidth;
	 clientRect.bottom = mWinHeight;
	 AdjustWindowRect(&clientRect, GetWindowLongPtr(mWindow, GWL_STYLE), FALSE);
	 if (clientRect.left < 0) {
	 int moveRight = -clientRect.left;
	 clientRect.left += moveRight;
	 clientRect.right += moveRight;
	 }
	 if (clientRect.top < 0) {
	 int moveDown = -clientRect.top;
	 clientRect.top += moveDown;
	 clientRect.bottom += moveDown;
	 }

	 MoveWindow(mWindow, clientRect.left, clientRect.top, clientRect.right,
	 clientRect.bottom, false);

	 // Hide the cursor
	 ShowCursor(false);

	 // Show the window
	 UpdateWindow(mWindow);
	 ShowWindow(mWindow, SW_SHOWNORMAL);*/

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
	glutIdleFunc(glutPostRedisplay);
	while (!windowClosed) {
		OnMainLoop();
		glutMainLoopEvent();
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
