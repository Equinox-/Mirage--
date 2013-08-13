//====================================================================================================
// Filename:	CTexture.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "CTexture.h"

#include "../Core/CLog.h"
#include "CDXGraphics.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

CTexture::CTexture(void) :
		mpTexture(NULL) {
	// Empty
}

//----------------------------------------------------------------------------------------------------

CTexture::~CTexture(void) {
	// Release everything
	Unload();
}

//----------------------------------------------------------------------------------------------------

void CTexture::Load(const char* pFilename) {
	// Clear everything before we create the texture
	Unload();

	// Struct for getting image info
	mpTexture = new GLTexture();

	// Load the texture from file
	int result = mpTexture->loadFromFile(pFilename);

	if (result != TEXTURE_LOAD_SUCCESS) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_ERROR,
				"[Texture] Failed to create texture from file %s", pFilename);
		return;
	}
}

//----------------------------------------------------------------------------------------------------

void CTexture::Unload(void) {
	// Release everything
	if (NULL != mpTexture) {
		mpTexture->freeRawData();
		mpTexture->freeTexture();
		mpTexture = NULL;
	}
}

//----------------------------------------------------------------------------------------------------

GLTexture* CTexture::GetTexture(void) const {
	return mpTexture;
}

//----------------------------------------------------------------------------------------------------

int CTexture::GetWidth(void) const {
	if (NULL == mpTexture) {
		return 0;
	}
	return mpTexture->getWidth();
}

//----------------------------------------------------------------------------------------------------

int CTexture::GetHeight(void) const {
	if (NULL == mpTexture) {
		return 0;
	}
	return mpTexture->getHeight();
}
