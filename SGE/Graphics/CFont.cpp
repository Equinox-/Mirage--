//====================================================================================================
// Filename:	CFont.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "CFont.h"

#include "../Core/CLog.h"
#include "CDXGraphics.h"
#include "CSpriteRenderer.h"
#include "../GLUtil/GLColor.h"
#include "../GLUtil/GLFont.h"
#include "../GLUtil/Math/Rectangle.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

CFont::CFont(void) :
		mColor(0xFFFFFFFF), font(NULL), mFormat(0)
//, mFormat(DT_LEFT | DT_TOP | DT_NOCLIP)
{
	mTextArea = Rectangle();
	// Set a default text area
	mTextArea.left = 0;
	mTextArea.top = 0;
	mTextArea.bottom = 1000;
	mTextArea.right = 1000;
}

//----------------------------------------------------------------------------------------------------

CFont::~CFont(void) {
	// Release everything
	Destroy();
}

//----------------------------------------------------------------------------------------------------

void CFont::Create(const char* pFontType, int iSize, bool bBold, bool bItalic) {
	font = new GLFont(0, 16, 16, "font.png", false, (float) iSize / 32.0);

	/*if (FAILED(hr))
	 {
	 // Write to log
	 CLog::Get()->Write(ELogMessageType_WARNING, "[Font] Failed to create Direct3D font.");
	 return;
	 }*/
}

//----------------------------------------------------------------------------------------------------

void CFont::Destroy(void) {
	// Release the font
	if (NULL != font) {
		font->dispose();
		font = NULL;
	}
}

//----------------------------------------------------------------------------------------------------

void CFont::SetColor(float iRed, float iGreen, float iBlue) {
	// Set color
	mColor = GLColor(iRed, iGreen, iBlue, 1.0);
}

//----------------------------------------------------------------------------------------------------

void CFont::SetTextArea(int iPosX, int iPosY, int iWidth, int iHeight) {
	// Validate parameters
	if (iPosX < 0) {
		iPosX = 0;
	}
	if (iPosY < 0) {
		iPosY = 0;
	}
	if (iWidth < 1) {
		iWidth = 1;
	}
	if (iHeight < 1) {
		iHeight = 1;
	}

	// Set the text area
	mTextArea.left = iPosX;
	mTextArea.top = iPosY;
	mTextArea.bottom = iPosY + iHeight;
	mTextArea.right = iPosX + iWidth;
}

//----------------------------------------------------------------------------------------------------

void CFont::SetFormat(int iFormat) {
	// Clear old format
	mFormat = 0;

	// Check for horizontal alignment
	/*if (iFormat & FontFormat::LEFT) {
	 mFormat |= DT_LEFT;
	 } else if (iFormat & FontFormat::CENTER) {
	 mFormat |= DT_CENTER;
	 } else if (iFormat & FontFormat::RIGHT) {
	 mFormat |= DT_RIGHT;
	 }*/

	// Check for vertical alignment
	/*if (iFormat & FontFormat::TOP) {
	 mFormat |= DT_TOP;
	 } else if (iFormat & FontFormat::MIDDLE) {
	 mFormat |= DT_VCENTER;
	 } else if (iFormat & FontFormat::BOTTOM) {
	 mFormat |= DT_BOTTOM;
	 }

	 // Check if single- or multi-line
	 if (iFormat & FontFormat::SINGLE) {
	 mFormat |= DT_SINGLELINE;
	 } else if (iFormat & FontFormat::MULTI) {
	 mFormat |= DT_WORDBREAK;
	 }*/

	// Always turn no-clip on
	//mFormat |= DT_NOCLIP;
}

//----------------------------------------------------------------------------------------------------

void CFont::PrintText(const char* pText, int iPosX, int iPosY) {
	if (NULL == font) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING,
				"[Font] Cannot draw text without a font.");
		return;
	}

	Rectangle rect = Rectangle();
	rect.left = mTextArea.left + iPosX;
	rect.right = mTextArea.right + iPosX;
	rect.top = mTextArea.top + iPosY;
	rect.bottom = mTextArea.bottom + iPosY;

	// Reset transform
	//D3DXMATRIX matIdentity;
	//D3DXMatrixIdentity(&matIdentity);
	//CSpriteRenderer::Get()->D3DXSprite()->SetTransform(&matIdentity);

	// Render the string
	/*mpFont->DrawTextA(CSpriteRenderer::Get()->D3DXSprite(),	// Sprite object for batching and optimization
	 pText,									// String to draw
	 -1,			// Character count (-1 = auto detect if null terminated)
	 &rect,									// Render area
	 mFormat,								// Format
	 mColor									// Color
	 );*/
	mColor.bind();
	font->render(pText, rect);
}
