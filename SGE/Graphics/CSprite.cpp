//====================================================================================================
// Filename:	CSprite.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "CSprite.h"

#include "../Core/CLog.h"
#include "CSpriteRenderer.h"
#include "CTexture.h"
#include "../Utility/Debug.h"
#include "../Misc/CIniFile.h"
#include "../GLUtil/Math/Matrix4f.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

CSprite::CSprite(void) :
		mColor(0xFFFFFFFF), mPosition(0.0f, 0.0f), mScale(1.0f, 1.0f), mRotation(
				0.0f), mDepth(0.0f), mScaleAboutCenter(true), mFlipH(false), mFlipV(
				false), mAnimationSpeed(0.0f), mAnimationTime(0.0f), mCurrentFrame(
				0), mPlay(false), mLoopOnce(false) {
	// Empty
}

//----------------------------------------------------------------------------------------------------

CSprite::~CSprite(void) {
	// Release everything
	ClearTextures();
}

//----------------------------------------------------------------------------------------------------

void CSprite::Update(float fSeconds) {
	// Update the animation time
	if (mPlay) {
		mAnimationTime += mAnimationSpeed * fSeconds;

		// Calculate the current frame to use
		const int kNumFrames = static_cast<int>(mpTextures.size());
		if (kNumFrames > 0) {
			const int kFrameIndex = static_cast<int>(mAnimationTime
					* mAnimationSpeed);
			if (mLoopOnce) {
				if (kFrameIndex >= kNumFrames - 1) {
					mCurrentFrame = kNumFrames - 1;
				} else {
					mCurrentFrame = kFrameIndex;
				}
			} else {
				mCurrentFrame = kFrameIndex % kNumFrames;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------

void CSprite::Render(void) {
	// Get the current texture to use
	const CTexture* pTexture = GetTexture();

	// Check if we have a valid texture
	if (NULL == pTexture) {
		return;
	}
	int WinWidth = CIniFile::Get()->GetInt("WinWidth", 800);
	if (mPosition.x > WinWidth || (mPosition.x + pTexture->GetWidth()) < 0)
		return;	// off screen

	int WinHeight = CIniFile::Get()->GetInt("WinHeight", 600);
	if (mPosition.y > WinHeight || (mPosition.y + pTexture->GetHeight()) < 0)
		return;	// off screen

	// Calculate texture center
	Vector2f vecTextureCenter(pTexture->GetWidth() * 0.5f,
			pTexture->GetHeight() * 0.5f);

	// Calculate sprite scale center
	Vector2f vecScaleCenter(0.0f, 0.0f);
	if (mScaleAboutCenter) {
		vecScaleCenter.x = vecTextureCenter.x;
		vecScaleCenter.y = vecTextureCenter.y;
	}

	// Calculate sprite rotation center
	Vector2f vecRotationCenter;
	vecRotationCenter.x = vecTextureCenter.x;
	vecRotationCenter.y = vecTextureCenter.y;
	if (!mScaleAboutCenter) {
		vecRotationCenter.x *= mScale.x;
		vecRotationCenter.y *= mScale.y;
	}

	float width = (float) pTexture->GetTexture()->getWidth();
	float height = (float) pTexture->GetTexture()->getHeight();
	pTexture->GetTexture()->bind();
	glPushMatrix();
	glTranslatef(mPosition.x, mPosition.y, 0.0);

	glTranslatef(vecScaleCenter.x, vecScaleCenter.y, 0.0);
	glScalef(mScale.x, mScale.y, 1.0);
	glTranslatef(-vecScaleCenter.x, -vecScaleCenter.y, 0.0);

	glTranslatef(vecRotationCenter.x, vecRotationCenter.y, 0.0);
	glRotatef(mRotation, 0.0, 0.0, 1.0);
	if (mFlipH) {
		glRotatef(180.0, 0.0, 1.0, 0.0);
	}
	if (mFlipV) {
		glRotatef(180.0, 1.0, 0.0, 0.0);
	}
	glTranslatef(-vecRotationCenter.x, -vecRotationCenter.y, 0.0);

	glBegin(GL_QUADS);
	mColor.bind();
	glTexCoord2f(0.0, 1.0);
	glVertex2f(0.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(width, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(width, height);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(0.0, height);
	glEnd();
	glPopMatrix();

#if 0
	// Draw original
	Vector2f pos(mPosition);
	Vector2f wh((float)pTexture->GetWidth(), (float)pTexture->GetHeight());
	Debug::AddScreenRect(pos.x, pos.y, wh.x, wh.y, 0xFFFFFF);

	// Draw center
	Debug::AddScreenLine(pos.x - 5.0f, pos.y - 5.0f, pos.x + 5.0f, pos.y + 5.0f, 0x8888FF);
	Debug::AddScreenLine(pos.x + 5.0f, pos.y - 5.0f, pos.x - 5.0f, pos.y + 5.0f, 0x8888FF);

	// Draw transformed
	Vector2f A(0.0f, 0.0f);
	Vector2f B(wh.x, 0.0f);
	Vector2f C(0.0f, wh.y);
	Vector2f D(wh.x, wh.y);
	D3DXVec2TransformCoord(&A, &A, &matCombined);
	D3DXVec2TransformCoord(&B, &B, &matCombined);
	D3DXVec2TransformCoord(&C, &C, &matCombined);
	D3DXVec2TransformCoord(&D, &D, &matCombined);
	Debug::AddScreenLine(A.x, A.y, B.x, B.y, 0xFF69B4);
	Debug::AddScreenLine(A.x, A.y, C.x, C.y, 0xFF69B4);
	Debug::AddScreenLine(D.x, D.y, B.x, B.y, 0xFF69B4);
	Debug::AddScreenLine(D.x, D.y, C.x, C.y, 0xFF69B4);

	// Draw transformed center
	Vector2f pos2(mPosition + vecRotationCenter);
	Debug::AddScreenLine(pos2.x - 5.0f, pos2.y - 5.0f, pos2.x + 5.0f, pos2.y + 5.0f, 0x88FFFF);
	Debug::AddScreenLine(pos2.x + 5.0f, pos2.y - 5.0f, pos2.x - 5.0f, pos2.y + 5.0f, 0x88FFFF);
#endif
}

//----------------------------------------------------------------------------------------------------

void CSprite::AddTexture(CTexture* pTexture) {
	mpTextures.push_back(pTexture);
}

//----------------------------------------------------------------------------------------------------

const CTexture* CSprite::GetTexture(void) const {
	CTexture* pTexture = NULL;
	if (mpTextures.size() > 0) {
		pTexture = mpTextures[mCurrentFrame];
	}
	return pTexture;
}

//----------------------------------------------------------------------------------------------------

void CSprite::ClearTextures(void) {
	mpTextures.clear();
}

//----------------------------------------------------------------------------------------------------

void CSprite::GetDimension(int& iWidth, int& iHeight) const {
	// Get the current frame to use
	const CTexture* pTexture = GetTexture();
	if (NULL != pTexture) {
		iWidth = static_cast<int>(pTexture->GetWidth() * mScale.x);
		iHeight = static_cast<int>(pTexture->GetHeight() * mScale.y);
	} else {
		iWidth = 0;
		iHeight = 0;
	}
}

//----------------------------------------------------------------------------------------------------

void CSprite::SetColor(int iRed, int iGreen, int iBlue, int iAlpha) {
	mColor = GLColor((float) iRed, (float) iGreen, (float) iBlue,
			(float) iAlpha);
}

//----------------------------------------------------------------------------------------------------

void CSprite::SetColor(int argb) {
	mColor.setValue(argb);
}

//----------------------------------------------------------------------------------------------------

void CSprite::GetPosition(float& fPosX, float& fPosY) const {
	fPosX = mPosition.x;
	fPosY = mPosition.y;
}

//----------------------------------------------------------------------------------------------------

void CSprite::GetScale(float& fScaleX, float& fScaleY) const {
	fScaleX = mScale.x;
	fScaleY = mScale.y;
}

//----------------------------------------------------------------------------------------------------

float CSprite::GetRotation(void) const {
	return mRotation;
}

//----------------------------------------------------------------------------------------------------

float CSprite::GetDepth(void) const {
	return mDepth;
}

//----------------------------------------------------------------------------------------------------

bool CSprite::IsScaleAboutCenter(void) const {
	return mScaleAboutCenter;
}

//----------------------------------------------------------------------------------------------------

bool CSprite::GetFlipH(void) const {
	return mFlipH;
}

//----------------------------------------------------------------------------------------------------

bool CSprite::GetFlipV(void) const {
	return mFlipV;
}

//----------------------------------------------------------------------------------------------------

void CSprite::SetPosition(float fPosX, float fPosY) {
	mPosition.x = fPosX;
	mPosition.y = fPosY;
}

//----------------------------------------------------------------------------------------------------

void CSprite::SetScale(float fScaleX, float fScaleY) {
	mScale.x = fScaleX;
	mScale.y = fScaleY;
}

//----------------------------------------------------------------------------------------------------

void CSprite::SetRotation(float fRotation) {
	mRotation = fRotation;
}

//----------------------------------------------------------------------------------------------------

void CSprite::SetDepth(float fDepth) {
	if (fDepth > 1.0f) {
		mDepth = 1.0f;
	} else if (fDepth < 0.0f) {
		mDepth = 0.0f;
	} else {
		mDepth = fDepth;
	}
}

//----------------------------------------------------------------------------------------------------

void CSprite::SetScaleAboutCenter(bool bScaleAboutCenter) {
	mScaleAboutCenter = bScaleAboutCenter;
}

//----------------------------------------------------------------------------------------------------

void CSprite::SetFlipH(bool bFlip) {
	mFlipH = bFlip;
}

//----------------------------------------------------------------------------------------------------

void CSprite::SetFlipV(bool bFlip) {
	mFlipV = bFlip;
}

//----------------------------------------------------------------------------------------------------

void CSprite::Play(bool bLoopOnce) {
	mPlay = true;
	mLoopOnce = bLoopOnce;
}

//----------------------------------------------------------------------------------------------------

void CSprite::Stop(void) {
	mPlay = false;
}

//----------------------------------------------------------------------------------------------------

void CSprite::Step(void) {
	const int kNumFrames = static_cast<int>(mpTextures.size());
	if (kNumFrames > 0) {
		mCurrentFrame = ++mCurrentFrame % kNumFrames;
	}
}

//----------------------------------------------------------------------------------------------------

void CSprite::Reset(void) {
	mAnimationTime = 0.0f;
	mCurrentFrame = 0;
}

//----------------------------------------------------------------------------------------------------

bool CSprite::IsFinished(void) const {
	if (!mLoopOnce) {
		return false;
	}

	const int kNumFrames = static_cast<int>(mpTextures.size());
	if (kNumFrames <= 0) {
		return false;
	}

	const float durationPerFrame = 1.0f / mAnimationSpeed;
	const float duration = durationPerFrame * kNumFrames;
	return mAnimationTime >= duration;
}

//----------------------------------------------------------------------------------------------------

float CSprite::GetAnimationSpeed(void) const {
	return mAnimationSpeed;
}

//----------------------------------------------------------------------------------------------------

int CSprite::GetCurrentFrame(void) const {
	return mCurrentFrame;
}

//----------------------------------------------------------------------------------------------------

void CSprite::SetAnimationSpeed(float fSpeed) {
	mAnimationSpeed = fSpeed;
}

//----------------------------------------------------------------------------------------------------

void CSprite::SetCurrentFrame(int iFrame) {
	const int kNumFrames = static_cast<int>(mpTextures.size());
	if (iFrame < kNumFrames) {
		mCurrentFrame = iFrame;
	} else {
		mCurrentFrame = kNumFrames - 1;
	}
}

void CSprite::SetAlpha(int alpha) {
	mColor.a = (float) alpha / 255.0;
}
