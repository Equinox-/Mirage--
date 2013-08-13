//====================================================================================================
// Filename:	SGE_Graphics.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "SGE_UI.h"

#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "Misc/Misc.h"
#include "Resource/Resource.h"

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE {

//====================================================================================================
// Class Definitions
//====================================================================================================

SGE_Font::SGE_Font(void) :
		mpFont(NULL) {
	mpFont = new CFont();
}

//----------------------------------------------------------------------------------------------------

SGE_Font::~SGE_Font(void) {
	delete mpFont;
}

//----------------------------------------------------------------------------------------------------

void SGE_Font::Load(int iSize, bool bBold, bool bItalic) {
	mpFont->Create(FontType::TAHOMA, iSize, bBold, bItalic);
}

//----------------------------------------------------------------------------------------------------

void SGE_Font::Unload(void) {
	mpFont->Destroy();
}

//----------------------------------------------------------------------------------------------------

void SGE_Font::SetColor(int r, int g, int b) {
	mpFont->SetColor(r, g, b);
}

//----------------------------------------------------------------------------------------------------

void SGE_Font::Print(const char* pText, int iPositionX, int iPositionY) {
	mpFont->PrintText(pText, iPositionX, iPositionY);
}

//----------------------------------------------------------------------------------------------------

SGE_Cursor::SGE_Cursor(void) :
		mpSprite(NULL), mpTexture(NULL), mShow(true) {
	mpSprite = new CSprite();
}

//----------------------------------------------------------------------------------------------------

SGE_Cursor::~SGE_Cursor(void) {
	delete mpSprite;
}

//----------------------------------------------------------------------------------------------------

void SGE_Cursor::Load(const char* pTextureName) {
	// Unload everything first
	Unload();

	// Construct the full filename
	std::string fullname;
	fullname += CIniFile::Get()->GetString("TexturePath", "../Data/Textures/");
	fullname += pTextureName;
	// Load the texture
	mpTexture = CResourceManager::Get()->Create<CTexture>(fullname.c_str());
	mpSprite->AddTexture(mpTexture);
}

//----------------------------------------------------------------------------------------------------

void SGE_Cursor::Unload(void) {
	// Clear texture reference
	mpSprite->ClearTextures();

	// Unload texture
	if (NULL != mpTexture) {
		CResourceManager::Get()->Destroy(mpTexture);
		mpTexture = NULL;
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Cursor::Update(float fSeconds) {
	float fMouseX = (float) CDXInput::Get()->GetMouseScreenX();
	float fMouseY = (float) CDXInput::Get()->GetMouseScreenY();
	mpSprite->SetPosition(fMouseX, fMouseY);
}

//----------------------------------------------------------------------------------------------------

void SGE_Cursor::Render(void) {
	if (mShow) {
		mpSprite->Render();
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Cursor::Show(void) {
	mShow = true;
}

//----------------------------------------------------------------------------------------------------

void SGE_Cursor::Hide(void) {
	mShow = false;
}

//----------------------------------------------------------------------------------------------------

SGE_Button::SGE_Button(void) :
		mpFont(NULL), mpSprite(NULL), mpTextureOn(NULL), mpTextureOff(NULL) {
	mpFont = new CFont();
	mpSprite = new CSprite();
}

//----------------------------------------------------------------------------------------------------

SGE_Button::~SGE_Button(void) {
	delete mpSprite;
	delete mpFont;
}

//----------------------------------------------------------------------------------------------------

void SGE_Button::Load(const char* pTextureOn, const char* pTextureOff,
		int iTextSize) {
	// Unload everything first
	Unload();

	// Construct the full filename
	std::string path;
	path = CIniFile::Get()->GetString("TexturePath", "../Data/Textures/");

	// Load textures
	std::string onTextureName;
	onTextureName = path + pTextureOn;
	mpTextureOn = CResourceManager::Get()->Create<CTexture>(
			onTextureName.c_str());

	std::string offTextureName;
	offTextureName = path + pTextureOff;
	mpTextureOff = CResourceManager::Get()->Create<CTexture>(
			offTextureName.c_str());

	// Add textures to sprite
	mpSprite->AddTexture(mpTextureOn);
	mpSprite->AddTexture(mpTextureOff);

	// Create font
	mpFont->Create(FontType::TAHOMA, iTextSize);
	mpFont->SetFormat(
			FontFormat::CENTER | FontFormat::MIDDLE | FontFormat::SINGLE);
	mpFont->SetTextArea(0, 0, mpTextureOn->GetWidth(),
			mpTextureOn->GetHeight());
}

//----------------------------------------------------------------------------------------------------

void SGE_Button::Unload(void) {
	// Destroy font
	mpFont->Destroy();

	// Clear texture reference
	mpSprite->ClearTextures();

	// Unload textures
	if (NULL != mpTextureOn) {
		CResourceManager::Get()->Destroy(mpTextureOn);
		mpTextureOn = NULL;
	}
	if (NULL != mpTextureOff) {
		CResourceManager::Get()->Destroy(mpTextureOff);
		mpTextureOff = NULL;
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Button::Update(float fSeconds) {
	const CTexture* pTexture = mpSprite->GetTexture();

	SRect boundary;
	mpSprite->GetPosition(boundary.min.x, boundary.min.y);
	boundary.max.x = boundary.min.x + pTexture->GetWidth();
	boundary.max.y = boundary.min.y + pTexture->GetHeight();

	SVector2 mouse((float) CDXInput::Get()->GetMouseScreenX(),
			(float) CDXInput::Get()->GetMouseScreenY());
	if (PointInRect(mouse, boundary)) {
		mpSprite->SetCurrentFrame(ON);
	} else {
		mpSprite->SetCurrentFrame(OFF);
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Button::Render(void) {
	mpSprite->Render();

	SVector2 vPos;
	mpSprite->GetPosition(vPos.x, vPos.y);

	if (mText.size() > 0) {
		mpFont->PrintText(mText.c_str(), (int) vPos.x, (int) vPos.y);
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Button::SetPosition(float x, float y) {
	mpSprite->SetPosition(x, y);
}

//----------------------------------------------------------------------------------------------------

void SGE_Button::SetPosition(const SVector2& pos) {
	mpSprite->SetPosition(pos.x, pos.y);
}

//----------------------------------------------------------------------------------------------------

void SGE_Button::SetText(const char* pText, int r, int g, int b) {
	mText = pText;
	mpFont->SetColor(r, g, b);
}

//----------------------------------------------------------------------------------------------------

bool SGE_Button::IsPressed(void) const {
	const CTexture* pTexture = mpSprite->GetTexture();

	SRect boundary;
	mpSprite->GetPosition(boundary.min.x, boundary.min.y);
	boundary.max.x = boundary.min.x + pTexture->GetWidth();
	boundary.max.y = boundary.min.y + pTexture->GetHeight();

	bool bPressed = false;
	SVector2 mouse((float) CDXInput::Get()->GetMouseScreenX(),
			(float) CDXInput::Get()->GetMouseScreenY());
	if (PointInRect(mouse, boundary)
			&& CDXInput::Get()->IsMousePressed(Mouse::LBUTTON)) {
		bPressed = true;
	}
	return bPressed;
}

} // namespace SGE
