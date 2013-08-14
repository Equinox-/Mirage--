//====================================================================================================
// Filename:	SGE.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Libraries
//====================================================================================================

//#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")
//#pragma comment(lib, "dinput8.lib")
//#pragma comment(lib, "dxguid.lib")
//#pragma comment(lib, "../SGE/External/Lib/FMOD/fmodex_vc.lib")
//#pragma comment (lib, "winmm.lib")

//====================================================================================================
// Includes
//====================================================================================================

#include <GL/freeglut.h>
#include "Audio/Audio.h"
#include "Core/Core.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "Misc/Misc.h"
#include "Resource/Resource.h"
#include "Utility/Utility.h"

#include <cstdarg>
#include <limits.h>
#include "SGE.h"

//====================================================================================================
// Externs
//====================================================================================================

extern void SGE_Initialize(void);
extern void SGE_Terminate(void);
extern bool SGE_Update(float fSeconds);
extern void SGE_Render(void);

//====================================================================================================
// Class Definitions
//====================================================================================================

class CGame: public CApplication {
protected:
	// Game initialization logic
	virtual void OnInitialize(void) {
		const int iWinWidth = CIniFile::Get()->GetInt("WinWidth", 800);
		const int iWinHeight = CIniFile::Get()->GetInt("WinHeight", 600);
		const int iResourceLimit = CIniFile::Get()->GetInt("ResourceLimit",
				100);
		const bool bFullscreen = CIniFile::Get()->GetBool("Fullscreen", false);

		// Initialize all engine systems
		CTimer::Get()->Initialize();
		CDXGraphics::Get()->Initialize(mWindow, iWinWidth, iWinHeight,
				bFullscreen);
		CSpriteRenderer::Get()->Initialize();
		CDXInput::Get()->Initialize(mWindow);
		CResourceManager::Get()->Initialize(iResourceLimit);
		CFMOD::Get()->Initialize(100);
		Random::SetRandSeed();

		// Debug initialization
		if (CIniFile::Get()->GetBool("Debug", false)) {
			Debug::Initialize();
		}

		// Create a new font
		mFont.Create(FontType::GARAMOND);
		mFont.SetColor(255, 255, 0);

		// User init logic
		SGE_Initialize();
	}

	// Game termination logic
	virtual void OnTerminate(void) {
		// User clean up logic
		SGE_Terminate();

		// Destroy the font
		mFont.Destroy();

		// Debug termination
		if (CIniFile::Get()->GetBool("Debug", false)) {
			Debug::Terminate();
		}

		// Shutdown all engine systems
		CFMOD::Get()->Terminate();
		CResourceManager::Get()->Terminate();
		CDXInput::Get()->Terminate();
		CSpriteRenderer::Get()->Terminate();
		CDXGraphics::Get()->Terminate();
		CTimer::Get()->Terminate();
	}

	// Game execution logic
	virtual void OnMainLoop(void) {
		SGE::Graphics_DebugUsageBegin("CoreUpdate");
		// Update timer
		CTimer::Get()->Update();
		float fSeconds = CTimer::Get()->GetElapsedSeconds();
		if (fSeconds > 0.0333f) {
			fSeconds = 0.0333f;
		}

		// Swap input buffers
		CDXInput::Get()->Update();

		// Update input buffer
		glutMainLoopEvent();

		// Update sound
		CFMOD::Get()->Update();

		SGE::Graphics_DebugUsageEnd();
		SGE::Graphics_DebugUsageBegin("UserUpdate");

		// User update logic
		if (SGE_Update(fSeconds)) {
			Quit();
			return;
		}

		// Begin scene
		CDXGraphics::Get()->BeginRender();

		// Begin sprite rendering
		CSpriteRenderer::Get()->BeginRender();

		SGE::Graphics_DebugUsageEnd();
		SGE::Graphics_DebugUsageBegin("UserRender");

		// User render logic
		SGE_Render();

		SGE::Graphics_DebugUsageEnd();

		// Render FPS
		const bool bFPS = CIniFile::Get()->GetBool("FPS", false);
		if (bFPS) {
			char temp[256];
			sprintf(temp, "FPS: %f", CTimer::Get()->GetFPS());
			mFont.PrintText(temp, 0, 0);
		}

		// End sprite rendering
		CSpriteRenderer::Get()->EndRender();

		// Debug render
		if (CIniFile::Get()->GetBool("Debug", false)) {
			Debug::Render();
		}

		// End scene
		CDXGraphics::Get()->EndRender();
	}

private:
	CFont mFont;
};

//====================================================================================================
// Main
//====================================================================================================

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	// Load init file
	CIniFile::Get()->Load("sge.ini");

	// Check if we are showing the splash screen
	/*const bool bSplashScreen = CIniFile::Get()->GetBool("SplashScreen", false);
	 if (bSplashScreen)
	 {
	 // Splash texture name
	 std::string fullname;
	 fullname += CIniFile::Get()->GetString("TexturePath", "../Data/Textures/");
	 fullname += "splashscreen.bmp";

	 // Splash screen
	 CSplashScreen splash;
	 splash.Initialize(fullname.c_str());
	 splash.Show(2000);
	 splash.Terminate();
	 }*/

	// Get initialization parameters // FIX TODO SACRILEGE
	char temp[CHAR_MAX];
	sprintf(temp, "www.touchofdeathforums.com  %s",
			CIniFile::Get()->GetString("AppName", "SGE"));
	const char* pAppName = temp;
	const int iWinWidth = CIniFile::Get()->GetInt("WinWidth", 800);
	const int iWinHeight = CIniFile::Get()->GetInt("WinHeight", 600);

	// Initialize and run the game
	CGame game;
	game.Initialize(pAppName, 0, iWinWidth, iWinHeight);
	game.Run();
	game.Terminate();

	// Unload init file
	CIniFile::Get()->Unload();

	return 0;
}

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE {

//====================================================================================================
// SGE Functions
//====================================================================================================

void LogMessage(const char* msg, ...) {
	va_list args;
	va_start(args, msg);
	CLog::Get()->Write(ELogMessageType_MESSAGE, msg, args);
	va_end(args);
}

//----------------------------------------------------------------------------------------------------

void LogWarning(const char* msg, ...) {
	va_list args;
	va_start(args, msg);
	CLog::Get()->Write(ELogMessageType_WARNING, msg, args);
	va_end(args);
}

//----------------------------------------------------------------------------------------------------

void LogError(const char* msg, ...) {
	va_list args;
	va_start(args, msg);
	CLog::Get()->Write(ELogMessageType_ERROR, msg, args);
	va_end(args);
}

//----------------------------------------------------------------------------------------------------

int IniFile_GetInt(const char* pTag, int iDefault) {
	return CIniFile::Get()->GetInt(pTag, iDefault);
}

//----------------------------------------------------------------------------------------------------

bool IniFile_GetBool(const char* pTag, bool bDefault) {
	return CIniFile::Get()->GetBool(pTag, bDefault);
}

//----------------------------------------------------------------------------------------------------

float IniFile_GetFloat(const char* pTag, float fDefault) {
	return CIniFile::Get()->GetFloat(pTag, fDefault);
}

//----------------------------------------------------------------------------------------------------

const char* IniFile_GetString(const char* pTag, const char* pDefault) {
	return CIniFile::Get()->GetString(pTag, pDefault);
}

//----------------------------------------------------------------------------------------------------

int RandomInt(int iLower, int iUpper) {
	return Random::GetInt(iLower, iUpper);
}

//----------------------------------------------------------------------------------------------------

float RandomFloat(float fLower, float fUpper) {
	return Random::GetFloat(fLower, fUpper);
}

//----------------------------------------------------------------------------------------------------

SVector2 RandomVector2(const SVector2& vLower, const SVector2& vUpper) {
	Vector2f lower(vLower.x, vLower.y);
	Vector2f upper(vUpper.x, vUpper.y);
	Vector2f vec(Random::GetVector2(lower, upper));

	return SVector2(vec.x, vec.y);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugLine(const SVector2& vStart, const SVector2& vEnd,
		unsigned int rgb) {
	Debug::AddScreenLine(vStart.x, vStart.y, vEnd.x, vEnd.y, rgb);
}

void Graphics_DebugUsageBegin(const char *name) {
	Debug::UsageBegin(name);
}

void Graphics_DebugUsageEnd() {
	Debug::UsageEnd();
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugLine(const SLineSegment& rLineSegment, unsigned int rgb) {
	Debug::AddScreenLine(rLineSegment.from.x, rLineSegment.from.y,
			rLineSegment.to.x, rLineSegment.to.y, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugRect(const SVector2& vPosition, const SVector2& vDimension,
		unsigned int rgb) {
	float x = vPosition.x;
	float y = vPosition.y;
	float w = vDimension.x;
	float h = vDimension.y;
	Debug::AddScreenRect(x, y, w, h, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugRect(const SRect& rRect, unsigned int rgb) {
	float x = rRect.min.x;
	float y = rRect.min.y;
	float w = rRect.max.x - rRect.min.x;
	float h = rRect.max.y - rRect.min.y;
	Debug::AddScreenRect(x, y, w, h, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugCircle(const SVector2& vCenter, float fRadius,
		unsigned int rgb) {
	Debug::AddScreenCircle(vCenter.x, vCenter.y, fRadius, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugCircle(const SCircle& rCircle, unsigned int rgb) {
	Debug::AddScreenCircle(rCircle.center.x, rCircle.center.y, rCircle.radius,
			rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugText(const char* pText, const SVector2& vPosition,
		unsigned int rgb) {
	Debug::AddScreenText(pText, vPosition.x, vPosition.y, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugText(const char* pText, float x, float y, unsigned int rgb) {
	Debug::AddScreenText(pText, x, y, rgb);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsKeyDown(int iKey) {
	return CDXInput::Get()->IsKeyDown(iKey);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsKeyPressed(int iKey) {
	return CDXInput::Get()->IsKeyPressed(iKey);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsMouseDown(int iMouse) {
	return CDXInput::Get()->IsMouseDown(iMouse);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsMousePressed(int iMouse) {
	return CDXInput::Get()->IsMousePressed(iMouse);
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseScreenX(void) {
	return CDXInput::Get()->GetMouseScreenX();
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseScreenY(void) {
	return CDXInput::Get()->GetMouseScreenY();
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseMoveX(void) {
	return false; //CDXInput::Get()->GetMouseMoveX();
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseMoveY(void) {
	return false; //CDXInput::Get()->GetMouseMoveY();
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseMoveZ(void) {
	return false; //CDXInput::Get()->GetMouseMoveZ();
}

//----------------------------------------------------------------------------------------------------

bool Input_IsGamePadButtonDown(int iButton) {
	return false; //CDXInput::Get()->IsGamePadButtonDown(iButton);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsGamePadButtonPressed(int iButton) {
	return false; //CDXInput::Get()->IsGamePadButtonPressed(iButton);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsDPadUp(void) {
	return false; //CDXInput::Get()->IsDPadUp();
}

//----------------------------------------------------------------------------------------------------

bool Input_IsDPadDown(void) {
	return false; //CDXInput::Get()->IsDPadDown();
}

//----------------------------------------------------------------------------------------------------

bool Input_IsDPadLeft(void) {
	return false; //CDXInput::Get()->IsDPadLeft();
}

//----------------------------------------------------------------------------------------------------

bool Input_IsDPadRight(void) {
	return false; //CDXInput::Get()->IsDPadRight();
}

//----------------------------------------------------------------------------------------------------

float Input_GetLeftAnalogX(void) {
	return 0.0; //CDXInput::Get()->GetLeftAnalogX();
}

//----------------------------------------------------------------------------------------------------

float Input_GetLeftAnalogY(void) {
	return 0.0; //CDXInput::Get()->GetLeftAnalogY();
}

//----------------------------------------------------------------------------------------------------

float Input_GetRightAnalogX(void) {
	return 0.0; //CDXInput::Get()->GetRightAnalogX();
}

//----------------------------------------------------------------------------------------------------

float Input_GetRightAnalogY(void) {
	return 0.0; //CDXInput::Get()->GetRightAnalogY();
}
static int GWinWidth = 0;
static int GWinHeight = 0;
int Graphics_WindowWidth() {
	if (GWinWidth == 0) {
		GWinWidth = glutGet(GLUT_WINDOW_WIDTH);
	}
	return GWinWidth;
}

int Graphics_WindowHeight() {
	if (GWinHeight == 0) {
		GWinHeight = glutGet(GLUT_WINDOW_HEIGHT);
	}
	return GWinHeight;
}

} // namespace SGE
