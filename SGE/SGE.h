#ifndef INCLUDED_SGE_H
#define INCLUDED_SGE_H

//====================================================================================================
// Filename:	SGE.h
// Created by:	Peter Chan
// Description:	Simple Game Engine.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <stdio.h>

#include "SGE_Audio.h"
#include "SGE_Graphics.h"
#include "SGE_Input.h"
#include "SGE_Math.h"
#include "SGE_UI.h"

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// SGE Functions
//====================================================================================================

// Error Log Functions
void		LogMessage(const char* msg, ...);
void		LogWarning(const char* msg, ...);
void		LogError(const char* msg, ...);

// INI File Functions
int			IniFile_GetInt(const char* pTag, int iDefault);
bool		IniFile_GetBool(const char* pTag, bool bDefault);
float		IniFile_GetFloat(const char* pTag, float fDefault);
const char*	IniFile_GetString(const char* pTag, const char* pDefault);

// Random Functions
int			RandomInt(int iLower, int iUpper);
float		RandomFloat(float fLower, float fUpper);
SVector2	RandomVector2(const SVector2& vLower, const SVector2& vUpper);

// Graphics System Functions
void Graphics_DebugLine(const SVector2& vStart, const SVector2& vEnd, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugLine(const SLineSegment& rLineSegment, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugRect(const SVector2& vPosition, const SVector2& vDimension, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugRect(const SRect& rRect, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugCircle(const SVector2& vCenter, float fRadius, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugCircle(const SCircle& rCircle, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugText(const char* pText, const SVector2& vPosition, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugText(const char* pText, float x, float y, unsigned int rgb = 0xFFFFFF);

// Keyboard Functions
bool Input_IsKeyDown(int iKey);
bool Input_IsKeyPressed(int iKey);

// Mouse Functions
bool Input_IsMouseDown(int iMouse);
bool Input_IsMousePressed(int iMouse);
int Input_GetMouseScreenX(void);
int Input_GetMouseScreenY(void);
int Input_GetMouseMoveX(void);
int Input_GetMouseMoveY(void);
int Input_GetMouseMoveZ(void);

// Gamepad Functions
bool Input_IsGamePadButtonDown(int iButton);
bool Input_IsGamePadButtonPressed(int iButton);
bool Input_IsDPadUp(void);
bool Input_IsDPadDown(void);
bool Input_IsDPadLeft(void);
bool Input_IsDPadRight(void);
float Input_GetLeftAnalogX(void);
float Input_GetLeftAnalogY(void);
float Input_GetRightAnalogX(void);
float Input_GetRightAnalogY(void);

} // namespace SGE

#endif // #ifndef INCLUDED_SGE_H