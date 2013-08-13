//====================================================================================================
// Filename:	SGE_Graphics.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "SGE_Graphics.h"

#include "Core/Core.h"
#include "Graphics/Graphics.h"
#include "Misc/Misc.h"
#include "Resource/Resource.h"
#include "Utility/Utility.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

class CParticle {
public:
	float px, py;
	float vx, vy;
	float lifetime;
	float size;
	int color;

	CParticle(void) :
			px(0.0f), py(0.0f), vx(0.0f), vy(0.0f), lifetime(0.0f), size(1.0f), color(
					0xFFFFFFFF) {
	}
};

//====================================================================================================
// Helper Functions
//====================================================================================================

int BlendColor(float s, int c0, int c1) {
	int r0, g0, b0;
	int r1, g1, b1;
	int rf, gf, bf;

	r0 = (c0 & 0x00FF0000) >> 16;
	g0 = (c0 & 0x0000FF00) >> 8;
	b0 = (c0 & 0x000000FF);

	r1 = (c1 & 0x00FF0000) >> 16;
	g1 = (c1 & 0x0000FF00) >> 8;
	b1 = (c1 & 0x000000FF);

	rf = r0 + static_cast<int>(s * (r1 - r0));
	gf = g0 + static_cast<int>(s * (g1 - g0));
	bf = b0 + static_cast<int>(s * (b1 - b0));

	return (0xFF << 24) | (rf << 16) | (gf << 8) | (bf);
}

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE {

//====================================================================================================
// Class Definitions
//====================================================================================================

SGE_Sprite::SGE_Sprite(void) {
	mpSprite = new CSprite();
}

//----------------------------------------------------------------------------------------------------

SGE_Sprite::~SGE_Sprite(void) {
	delete mpSprite;
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Load(const char* pTextureName) {
	// Unload everything first
	Unload();

	// Construct the full filename
	std::string fullname;
	fullname += CIniFile::Get()->GetString("TexturePath", "../Data/Textures/");
	fullname += pTextureName;

	// Load the texture
	CTexture* pTexture = CResourceManager::Get()->Create<CTexture>(
			fullname.c_str());
	mpTextures.push_back(pTexture);
	mpSprite->AddTexture(pTexture);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Add(const char* pTextureName) {
	// Construct the full filename
	std::string fullname;
	fullname += CIniFile::Get()->GetString("TexturePath", "../Data/Textures/");
	fullname += pTextureName;

	// Load the texture
	CTexture* pTexture = CResourceManager::Get()->Create<CTexture>(
			fullname.c_str());
	mpTextures.push_back(pTexture);
	mpSprite->AddTexture(pTexture);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Unload(void) {
	// Clear texture reference
	mpSprite->ClearTextures();

	// Unload texture
	const int kNumTextures = mpTextures.size();
	for (int i = 0; i < kNumTextures; ++i) {
		if (NULL != mpTextures[i]) {
			CResourceManager::Get()->Destroy(mpTextures[i]);
			mpTextures[i] = NULL;
		}
	}
	mpTextures.clear();
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Update(float fSeconds) {
	mpSprite->Update(fSeconds);
}

//----------------------------------------------------------------------------------------------------
// inline this
void SGE_Sprite::Render(void) {
	mpSprite->Render();
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetPosition(float x, float y) {
	mpSprite->SetPosition(x, y);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetPosition(const SVector2& pos) {
	mpSprite->SetPosition(pos.x, pos.y);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetScale(float x, float y, bool scaleAboutCenter) {
	mpSprite->SetScaleAboutCenter(scaleAboutCenter);
	mpSprite->SetScale(x, y);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetScale(const SVector2& scale, bool scaleAboutCenter) {
	mpSprite->SetScaleAboutCenter(scaleAboutCenter);
	mpSprite->SetScale(scale.x, scale.y);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetRotation(float rotation) {
	mpSprite->SetRotation(rotation);
}

void SGE_Sprite::SetScaleAboutCenter(bool scaleAboutCenter) {
	mpSprite->SetScaleAboutCenter(scaleAboutCenter);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetFlipH(bool bFlip) {
	mpSprite->SetFlipH(bFlip);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetFlipV(bool bFlip) {
	mpSprite->SetFlipV(bFlip);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Play(float speed, bool loop) {
	mpSprite->SetAnimationSpeed(speed);
	mpSprite->Play(!loop);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Stop(void) {
	mpSprite->Stop();
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Reset(void) {
	mpSprite->Reset();
}

//----------------------------------------------------------------------------------------------------

int SGE_Sprite::GetWidth(void) const {
	return mpSprite->GetTexture()->GetWidth();
}

//----------------------------------------------------------------------------------------------------

int SGE_Sprite::GetHeight(void) const {
	return mpSprite->GetTexture()->GetHeight();
}

//----------------------------------------------------------------------------------------------------

bool SGE_Sprite::IsFinished(void) const {
	return mpSprite->IsFinished();
}

//----------------------------------------------------------------------------------------------------

int SGE_Sprite::GetCurrentFrame(void) const {
	return mpSprite->GetCurrentFrame();
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetCurrentFrame(int iFrame) {
	mpSprite->SetCurrentFrame(iFrame);
}

void SGE_Sprite::SetAlpha(int alpha) {
	mpSprite->SetAlpha(alpha);
}

void SGE_Sprite::SetColor(int iRed, int iGreen, int iBlue, int iAlpha) {
	mpSprite->SetColor(iRed, iGreen, iBlue, iAlpha);
}

//----------------------------------------------------------------------------------------------------

SGE_Emitter::SGE_Emitter(void) :
		mpSprite(NULL), mpTexture(NULL), mParticles(NULL), mTimer(0.0f), mRate(
				0.0f), mInterval(1.0f), mEmitted(0.0f), mIndex(0), mPosition(
				0.0f, 0.0f), mOffset(0.0f, 0.0f), mFire(false), mBurst(false), mMaxBufferSize(
				0), mRegion(0.0f, 0.0f, 0.0f, 0.0f), mDirection(0.0f, -1.0f), mSpan(
				180.0f), mSpeedMin(0.0f), mSpeedMax(0.0f), mLifetime(1.0f), mScaleMin(
				1.0f), mScaleMax(1.0f), mStartColor(0xFFFFFFFF), mEndColor(
				0xFFFFFFFF) {
	mpSprite = new CSprite();
}

//----------------------------------------------------------------------------------------------------

SGE_Emitter::~SGE_Emitter(void) {
	delete mpSprite;
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::Load(const char* pEmitterName) {
	// Unload everything first
	Unload();

	// Full name
	std::string fullname;
	fullname += CIniFile::Get()->GetString("EmitterPath", "../Data/Emitters/");
	fullname += pEmitterName;

	// Open the emitter file
	FILE* pFile = NULL;
	pFile = fopen(fullname.c_str(), "r");
	if (NULL == pFile) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_ERROR,
				"[Emitter] Failed to load emitter file %s.", pEmitterName);
		return;
	}

	// Validate the file
	const int kBufferSize = 256;
	char data[kBufferSize];
	fscanf(pFile, "%s", data);
	if (0 != strcmp(data, "[EMITTER_DATA]")) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_ERROR,
				"[Emitter] Invalid emitter file %s.", fullname.c_str());
		fclose(pFile);
		return;
	}

	// Now read the data
	char texture[kBufferSize];
	int r0, g0, b0;
	int r1, g1, b1;

	// Read the emitter data
	fscanf(pFile, "%*s %s", texture);
	fscanf(pFile, "%*s %i", &mMaxBufferSize);
	fscanf(pFile, "%*s %f, %f, %f, %f", &mRegion.min.x, &mRegion.min.y,
			&mRegion.max.x, &mRegion.max.y);
	fscanf(pFile, "%*s %f, %f", &mDirection.x, &mDirection.y);
	fscanf(pFile, "%*s %f", &mSpan);
	fscanf(pFile, "%*s %f, %f", &mSpeedMin, &mSpeedMax);
	fscanf(pFile, "%*s %f", &mLifetime);
	fscanf(pFile, "%*s %f, %f", &mScaleMin, &mScaleMax);
	fscanf(pFile, "%*s %d, %d, %d", &r0, &g0, &b0);
	fscanf(pFile, "%*s %d, %d, %d", &r1, &g1, &b1);

	// Close the data file
	fclose(pFile);

	// Pack color
	mStartColor = (0xFF << 24) | (r0 << 16) | (g0 << 8) | (b0);
	mEndColor = (0xFF << 24) | (r1 << 16) | (g1 << 8) | (b1);

	// Allocate memory for particles
	mParticles = new CParticle[mMaxBufferSize];

	// Load and set texture
	fullname = CIniFile::Get()->GetString("TexturePath", "../Data/Textures/");
	fullname += texture;
	mpTexture = CResourceManager::Get()->Create<CTexture>(fullname.c_str());
	mpSprite->AddTexture(mpTexture);
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::Unload(void) {
	// Clear texture reference
	mpSprite->ClearTextures();

	// Unload texture
	if (NULL != mpTexture) {
		CResourceManager::Get()->Destroy(mpTexture);
		mpTexture = NULL;
	}

	// Delete particles
	if (NULL != mParticles) {
		delete[] mParticles;
		mParticles = NULL;
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::Update(float fSeconds) {
	if (mFire) {
		int iSlotSearched = 0;
		while (mEmitted < mRate && iSlotSearched < mMaxBufferSize) {
			CParticle& p = mParticles[mIndex];

			// Check if slot is available
			if (p.lifetime <= 0.0f) {
				// Calculate parameters
				SVector2 vecPositionMin = mPosition + mRegion.min;
				SVector2 vecPositionMax = mPosition + mRegion.max;

				float fAngle = Random::GetFloat(-mSpan, mSpan);
				float fSpeed = Random::GetFloat(mSpeedMin, mSpeedMax);

				SVector2 vecDirection(mDirection);
				vecDirection.Rotate(DegToRad(fAngle));
				vecDirection.Normalize();

				// Set particle attributes
				p.px = Random::GetFloat(vecPositionMin.x, vecPositionMax.x);
				p.py = Random::GetFloat(vecPositionMin.y, vecPositionMax.y);
				p.vx = vecDirection.x * fSpeed;
				p.vy = vecDirection.y * fSpeed;
				p.size = Random::GetFloat(mScaleMin, mScaleMax);
				p.color = mStartColor;
				p.lifetime = mLifetime;

				// Increment counter
				mEmitted += 1.0f;
			}

			// Advance index
			mIndex = (mIndex + 1) % mMaxBufferSize;
			++iSlotSearched;
		}

		// Check if this is burst mode
		if (mBurst) {
			// Reset
			mFire = false;
			mBurst = false;
			mTimer = 0.0f;
			mEmitted = 0.0f;
		} else {
			mTimer += fSeconds;

			if (mTimer > mInterval) {
				mTimer -= mInterval;
				mEmitted = 0.0f;
			}
		}
	}

	for (int i = 0; i < mMaxBufferSize; ++i) {
		CParticle& p = mParticles[i];
		if (p.lifetime > 0.0f) {
			// Update lifetime
			p.lifetime -= fSeconds;

			float s = 1.0f - (p.lifetime / mLifetime);

			// Update attributes
			p.px += p.vx * fSeconds;
			p.py += p.vy * fSeconds;
			p.color = BlendColor(s, mStartColor, mEndColor);
		}
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::Render(bool bAdditiveBlend) {
	if (bAdditiveBlend) {
		// Switch to additive blending
		//CSpriteRenderer::Get()->D3DXSprite()->Flush();
		//CDXGraphics::Get()->D3DDevice()->SetRenderState(D3DRS_SRCBLEND,
		//		D3DBLEND_ONE);
		//CDXGraphics::Get()->D3DDevice()->SetRenderState(D3DRS_DESTBLEND,
		//		D3DBLEND_ONE);
	}

	for (int i = 0; i < mMaxBufferSize; ++i) {
		CParticle& p = mParticles[i];
		if (p.lifetime > 0.0f) {
			mpSprite->SetPosition(p.px + mOffset.x, p.py + mOffset.y);
			mpSprite->SetScale(p.size, p.size);
			mpSprite->SetColor(p.color);
			mpSprite->Render();
		}
	}

	if (bAdditiveBlend) {
		// Revert to default blending
		/*CSpriteRenderer::Get()->D3DXSprite()->Flush();
		 CDXGraphics::Get()->D3DDevice()->SetRenderState(D3DRS_SRCBLEND,
		 D3DBLEND_SRCALPHA);
		 CDXGraphics::Get()->D3DDevice()->SetRenderState(D3DRS_DESTBLEND,
		 D3DBLEND_INVSRCALPHA);*/
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::Start(float rate, bool burst) {
	if (burst) {
		mRate = rate;
		mInterval = 1.0f;
	} else {
		//mRate = 1.0f;
		mRate = rate;
		mInterval = 1.0f / rate;
	}

	mFire = true;
	mBurst = burst;
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::Stop(void) {
	mTimer = 0.0f;
	mEmitted = 0.0f;

	mFire = false;
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::SetPosition(float x, float y) {
	mPosition.x = x;
	mPosition.y = y;
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::SetPosition(const SVector2& pos) {
	mPosition = pos;
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::SetOffset(float x, float y) {
	mOffset.x = x;
	mOffset.y = y;
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::SetOffset(SVector2 pos) {
	mOffset = pos;
}

//----------------------------------------------------------------------------------------------------

SGE_WireFrame::SGE_WireFrame(void) :
		mWireFrame(NULL) {
	mWireFrame = new CWireFrame();
	mWireFrame->Create();
}

//----------------------------------------------------------------------------------------------------

SGE_WireFrame::SGE_WireFrame(const SGE_WireFrame& rhs) :
		mWireFrame(NULL) {
	mWireFrame = new CWireFrame(*rhs.mWireFrame);
	mWireFrame->Create();
}

//----------------------------------------------------------------------------------------------------

SGE_WireFrame::~SGE_WireFrame(void) {
	mWireFrame->Destroy();
	delete mWireFrame;
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::operator=(const SGE_WireFrame& rhs) {
	memcpy(&mWireFrame, &rhs.mWireFrame, sizeof(CWireFrame));
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::Render(void) {
	mWireFrame->Render();
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddLine(float x0, float y0, float x1, float y1) {
	mWireFrame->AddLine(x0, y0, x1, y1);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddLine(const SVector2& start, const SVector2& end) {
	mWireFrame->AddLine(start.x, start.y, end.x, end.y);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddRect(float x, float y, float w, float h) {
	mWireFrame->AddRect(x, y, w, h);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddRect(const SVector2& pos, const SVector2& dim) {
	mWireFrame->AddRect(pos.x, pos.y, dim.x, dim.y);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddRect(const SRect& rRect) {
	mWireFrame->AddRect(rRect.min.x, rRect.min.y, rRect.max.x - rRect.min.x,
			rRect.max.y - rRect.min.y);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddCircle(float x, float y, float r) {
	mWireFrame->AddCircle(x, y, r);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddCircle(const SVector2& c, float r) {
	mWireFrame->AddCircle(c.x, c.y, r);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddCircle(const SCircle& rCircle) {
	mWireFrame->AddCircle(rCircle.center.x, rCircle.center.y, rCircle.radius);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::Purge(void) {
	mWireFrame->Purge();
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::SetLineWidth(float fLineWidth) {
	mWireFrame->SetLineWidth(fLineWidth);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::SetColor(unsigned int rgb) {
	mWireFrame->SetColor(rgb);
}

} // namespace SGE
