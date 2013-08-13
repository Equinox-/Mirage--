#ifndef INCLUDED_FONT_H
#define INCLUDED_FONT_H

#include "../GLUtil/GLColor.h"
#include "../GLUtil/GLFont.h"
#include "../GLUtil/Math/Rectangle.h"
//====================================================================================================
// Filename:	CFont.h
// Created by:	Peter Chan
// Description:	Class for a Direct3D font.
//====================================================================================================

//====================================================================================================
// Constants
//====================================================================================================

namespace FontType {
static const char* ARIAL = "Arial";
static const char* COURIER = "Courier New";
static const char* GARAMOND = "Garamond";
static const char* IMPACT = "Impact";
static const char* TAHOMA = "Tahoma";
static const char* TIMES = "Times New Roman";
static const char* VERDANA = "Verdana";
}

namespace FontFormat {
static const int LEFT = 0x1 << 0;
static const int CENTER = 0x1 << 1;
static const int RIGHT = 0x1 << 2;
static const int TOP = 0x1 << 3;
static const int MIDDLE = 0x1 << 4;
static const int BOTTOM = 0x1 << 5;
static const int SINGLE = 0x1 << 6;
static const int MULTI = 0x1 << 7;
}

//====================================================================================================
// Includes
//====================================================================================================

//====================================================================================================
// Class Declarations
//====================================================================================================

class CFont {
public:
	// Constructor
	CFont(void);

	// Destructor
	~CFont(void);

	// Function to create/destroy a new font
	void Create(const char* pFontType, int iSize = 24, bool bBold = false,
			bool bItalic = false);
	void Destroy(void);

	// Function to set the text color
	void SetColor(float iRed, float iGreen, float iBlue);

	// Function to set the text area
	void SetTextArea(int iPosX, int iPosY, int iWidth, int iHeight);

	// Function to set the text format
	void SetFormat(int iFormat);

	// Function to draw text
	void PrintText(const char* pText, int iPosX = 0, int iPosY = 0);

private:
	//ID3DXFont*	  mpFont;	// Pointer to Direct3D font interface

	GLColor mColor;		// Text color
	Rectangle mTextArea;	 // Text area
	GLFont *font;
	int mFormat;	   // Text format
};

#endif // #ifndef INCLUDED_FONT_H
