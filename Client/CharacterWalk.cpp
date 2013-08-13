//====================================================================================================
// Includes
//====================================================================================================

#include "CharacterWalk.h"

#include "Character.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

CharacterWalk::CharacterWalk(Character* pOwner, Map& map,
		PlayerInfo& playerInfo, Raknet& raknet) :
		CharacterState(pOwner, "Walk", playerInfo, raknet), mMap(map), mSpriteWalkAnimations(
				0), mMoved(false) {
	// Empty
}

//----------------------------------------------------------------------------------------------------

CharacterWalk::~CharacterWalk() {
	// Empty
}

//----------------------------------------------------------------------------------------------------

void CharacterWalk::Load() {
	mSprite.Unload();
	mSpriteWalkAnimations = IniFile_GetInt("SPRITE_WALK_ANIMATIONS", 16);
	char temp[CHAR_MAX];
	for (int a = 1; a < mSpriteWalkAnimations; ++a) {
		sprintf(temp, "Sprites/%d/%d.png", mPlayerInfo.GetSpriteNumber(), a);
		mSprite.Add(temp);
	}

	// Get the right frames
	mFrames.WalkDownStart = IniFile_GetInt("WALK_DOWN_START", 1);
	mFrames.WalkDownEnd = IniFile_GetInt("WALK_DOWN_END", 4);
	mFrames.WalkLeftStart = IniFile_GetInt("WALK_LEFT_START", 5);
	mFrames.WalkLeftEnd = IniFile_GetInt("WALK_LEFT_END", 8);
	mFrames.WalkUpStart = IniFile_GetInt("WALK_UP_START", 9);
	mFrames.WalkUpEnd = IniFile_GetInt("WALK_UP_END", 12);
}

//----------------------------------------------------------------------------------------------------

void CharacterWalk::Unload() {
	mSprite.Unload();
}

//----------------------------------------------------------------------------------------------------

bool CharacterWalk::Update(float fSeconds) {
	mMoved = false;
	mSprite.Update(fSeconds);
	mAnimation.UpdateTime(fSeconds);

	// State transitions
	const float kSpeed = 250.0f;

	/*
	 if(Input_IsKeyDown(Keys::LCONTROL))
	 {
	 mpOwner->SetVelocity(SVector2());
	 mpOwner->ChangeState(AS_Combat);
	 }
	 */

	if (Input_IsKeyDown(Keys::UP) && Input_IsKeyDown(Keys::LEFT)) {
		SVector2 vel(-kSpeed * fSeconds, -kSpeed * fSeconds);
		mpOwner->SetVelocity(vel);
		mAnimation.SetStartFrame(mFrames.WalkLeftStart);
		mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
		mpOwner->SetDirection(leftUp);
		mpOwner->SetFacingLeft(false);
		mMoved = true;
	} else if (Input_IsKeyDown(Keys::DOWN) && Input_IsKeyDown(Keys::LEFT)) {
		SVector2 vel(-kSpeed * fSeconds, kSpeed * fSeconds);
		mpOwner->SetVelocity(vel);
		mAnimation.SetStartFrame(mFrames.WalkLeftStart);
		mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
		mpOwner->SetDirection(leftDown);
		mpOwner->SetFacingLeft(false);
		mMoved = true;
	} else if (Input_IsKeyDown(Keys::DOWN) && Input_IsKeyDown(Keys::RIGHT)) {
		SVector2 vel(kSpeed * fSeconds, kSpeed * fSeconds);
		mpOwner->SetVelocity(vel);
		mAnimation.SetStartFrame(mFrames.WalkLeftStart);
		mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
		mpOwner->SetDirection(rightDown);
		mpOwner->SetFacingLeft(true);
		mMoved = true;
	} else if (Input_IsKeyDown(Keys::UP) && Input_IsKeyDown(Keys::RIGHT)) {
		SVector2 vel(kSpeed * fSeconds, -kSpeed * fSeconds);
		mpOwner->SetVelocity(vel);
		mAnimation.SetStartFrame(mFrames.WalkLeftStart);
		mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
		mpOwner->SetDirection(rightUp);
		mpOwner->SetFacingLeft(true);
		mMoved = true;
	} else if (Input_IsKeyDown(Keys::UP)) {
		SVector2 vel(0.0f, -kSpeed * fSeconds);
		mpOwner->SetVelocity(vel);
		mpOwner->SetDirection(up);

		mAnimation.SetStartFrame(mFrames.WalkUpStart);
		mAnimation.SetEndFrame(mFrames.WalkUpEnd);
		mMoved = true;
	} else if (Input_IsKeyDown(Keys::DOWN)) {
		SVector2 vel(0.0f, kSpeed * fSeconds);
		mpOwner->SetVelocity(vel);
		mpOwner->SetDirection(down);

		mAnimation.SetStartFrame(mFrames.WalkDownStart);
		mAnimation.SetEndFrame(mFrames.WalkDownEnd);
		mMoved = true;
	} else if (Input_IsKeyDown(Keys::RIGHT)) {
		SVector2 vel(kSpeed * fSeconds, 0.0f);
		mpOwner->SetVelocity(vel);
		mpOwner->SetFacingLeft(true);
		mpOwner->SetDirection(right);

		mAnimation.SetStartFrame(mFrames.WalkLeftStart);
		mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
		mMoved = true;
	} else if (Input_IsKeyDown(Keys::LEFT)) {
		SVector2 vel(-kSpeed * fSeconds, 0.0f);
		mpOwner->SetVelocity(vel);
		mpOwner->SetFacingLeft(false);
		mpOwner->SetDirection(left);

		mAnimation.SetStartFrame(mFrames.WalkLeftStart);
		mAnimation.SetEndFrame(mFrames.WalkLeftEnd);

		mMoved = true;
	} else {
		mpOwner->SetVelocity(SVector2());
		mpOwner->ChangeState(AS_Idle);
	}

	mAnimation.AnimateSprite(fSeconds, mSprite);
	mAnimation.SetIsAnimating(true);

	//Graphics_DebugText("Current State: Walking", 0,150, 0xFFFFFF);

	return mMoved;
}

//----------------------------------------------------------------------------------------------------

void CharacterWalk::Render(const SVector2& offset) {
	const int kTextureWidth = mSprite.GetWidth();
	const int kTextureHeight = mSprite.GetHeight();
	const SVector2 ownerPos(mpOwner->GetPosition());
	const SVector2 renderPos(ownerPos.x - (kTextureWidth * 0.5f),
			ownerPos.y - kTextureHeight);
	bool facingLeft = mpOwner->IsFacingLeft();

	SVector2 newPosition = renderPos + offset;
	mSprite.SetPosition(newPosition);
	mSprite.SetFlipH(facingLeft);
	mSprite.Render();

	mpOwner->GetFont().Print(mPlayerInfo.GetUsername(), newPosition.x,
			newPosition.y - 20);
}

//----------------------------------------------------------------------------------------------------

void CharacterWalk::Enter() {

}

//----------------------------------------------------------------------------------------------------

void CharacterWalk::Exit() {
	mSprite.Stop();
	mSprite.Reset();
}

//----------------------------------------------------------------------------------------------------

SRect CharacterWalk::GetBoundingBox() const {
	const SVector2 pos(mpOwner->GetPosition());
	return SRect(pos.x - 16.0f, pos.y - 56.0f, pos.x + 16.0f, pos.y);
}

