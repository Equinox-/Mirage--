#ifndef INCLUDED_SPLASHSCREEN_H
#define INCLUDED_SPLASHSCREEN_H

//====================================================================================================
// Filename:	CSplashScreen.h
// Created by:	Peter Chan
// Description:	Class for creating a splash screen. Internally, it uses windows GDI for rendering and
//			  can display any BMP file passed into Initialize().
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================
#if 0
#include <windows.h>

//====================================================================================================
// Class Declarations
//====================================================================================================

class CSplashScreen  
{
public:
	// Constructor
	CSplashScreen(void);

	// Destructor
	virtual ~CSplashScreen(void);

	// Functions to initialize/terminate the splash screen
	void Initialize(const char* pFilename);
	void Terminate(void);

	// Functions to show/hide the splash screen
	void Show(int iMilliSeconds = 0);
	void Hide();

	// Windows Message Handler
	LRESULT CALLBACK SplashWindowProc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);

private:
	// Function called when WM_PAINT message is received
	void OnPaint(void);

	// Members
	HWND	mSplashWindow;
	HBITMAP mBitmap;

	int mWinWidth;
	int mWinHeight;
};

#endif // #ifndef INCLUDED_SPLASHSCREEN_H
#endif
