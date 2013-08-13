#ifndef INCLUDED_SGE_UI_H
#define INCLUDED_SGE_UI_H

//====================================================================================================
// Filename:	SGE_UI.h
// Created by:	Peter Chan
// Description:	Header containing some useful classes for user interface.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <string>

#include "SGE_Math.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class CFont;
class CSprite;
class CTexture;

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// Class Declarations
//====================================================================================================

class SGE_Font
{
public:
	SGE_Font(void);
	~SGE_Font(void);

	void Load(int iSize, bool bBold = false, bool bItalic = false);
	void Unload(void);

	void SetColor(int r, int g, int b);
	void Print(const char* pText, int iPositionX, int iPositionY);

private:
	CFont* mpFont;
};

//----------------------------------------------------------------------------------------------------

class SGE_Cursor
{
public:
	SGE_Cursor(void);
	~SGE_Cursor(void);

	void Load(const char* pTextureName);
	void Unload(void);

	void Update(float fSeconds);
	void Render(void);

	void Show(void);
	void Hide(void);

private:
	CSprite*	mpSprite;
	CTexture*	mpTexture;
	bool		mShow;
};

//----------------------------------------------------------------------------------------------------

class SGE_Button
{
public:
	SGE_Button(void);
	~SGE_Button(void);

	void Load(const char* pTextureOn, const char* pTextureOff, int iTextSize = 24);
	void Unload(void);

	void Update(float fSeconds);
	void Render(void);

	void SetPosition(float x, float y);
	void SetPosition(const SVector2& pos);
	void SetText(const char* pText, int r, int g, int b);

	bool IsPressed(void) const;

private:
	enum{ ON = 0, OFF = 1 };

	CFont*		mpFont;
	CSprite*	mpSprite;
	CTexture*	mpTextureOn;
	CTexture*	mpTextureOff;
	std::string	mText;
};

} // namespace SGE

#endif // #ifndef INCLUDED_SGE_UI_H