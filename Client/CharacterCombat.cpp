//====================================================================================================
// Includes
//====================================================================================================

#include "CharacterCombat.h"

#include "Character.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

CharacterCombat::CharacterCombat(Character* pOwner, PlayerInfo& playerInfo,
		Raknet& raknet) :
		CharacterState(pOwner, "Idle", playerInfo, raknet), mPlayerInfo(
				playerInfo) {
	// Empty
}

//----------------------------------------------------------------------------------------------------

CharacterCombat::~CharacterCombat() {
	// Empty
}

//----------------------------------------------------------------------------------------------------

void CharacterCombat::Load() {
	mSprite.Unload();
	char temp[CHAR_MAX];

	// Get the right frames
	mFrames.WalkDownStart = IniFile_GetInt("WALK_DOWN_START", 1);
	mFrames.WalkLeftStart = IniFile_GetInt("WALK_LEFT_START", 5);
	mFrames.WalkUpStart = IniFile_GetInt("WALK_UP_START", 9);

	sprintf(temp, "Sprites/%d/%d.png", mPlayerInfo.GetSpriteNumber(),
			mFrames.WalkDownStart);
	mSprite.Add(temp);
	sprintf(temp, "Sprites/%d/%d.png", mPlayerInfo.GetSpriteNumber(),
			mFrames.WalkLeftStart);
	mSprite.Add(temp);
	sprintf(temp, "Sprites/%d/%d.png", mPlayerInfo.GetSpriteNumber(),
			mFrames.WalkUpStart);
	mSprite.Add(temp);
}

//----------------------------------------------------------------------------------------------------

void CharacterCombat::Unload() {
	mSprite.Unload();
}

//----------------------------------------------------------------------------------------------------

bool CharacterCombat::Update(float fSeconds) {
	// Update sprite
	mSprite.Update(fSeconds);

	// Switch to walk state if arrow keys are pressed 
	if (Input_IsKeyDown(Keys::RIGHT) || Input_IsKeyDown(Keys::LEFT)
			|| Input_IsKeyDown(Keys::UP) || Input_IsKeyDown(Keys::DOWN)) {
		mpOwner->ChangeState(AS_Walk);
	}

	// Switch to action state if control is pressed
	if (Input_IsKeyPressed(Keys::LCONTROL) && !mAnimation.GetIsAnimating()) {
		//mpOwner->ChangeState(AS_Action);
	}

	Graphics_DebugText("Current State: Combat", 0, 150, 0xFFFFFF);

	return false;
}

//----------------------------------------------------------------------------------------------------

void CharacterCombat::Render(const SVector2& offset) {
	switch (mpOwner->GetDirection()) {
	case left: {
		mSprite.SetCurrentFrame(1);
		mpOwner->SetFacingLeft(false);
		break;
	}
	case right: {
		mSprite.SetCurrentFrame(1);
		mpOwner->SetFacingLeft(true);
		break;
	}
	case up: {
		mSprite.SetCurrentFrame(2);
		break;
	}
	case down: {
		mSprite.SetCurrentFrame(0);
		break;
	}
	}

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

void CharacterCombat::Enter() {

}

//----------------------------------------------------------------------------------------------------

void CharacterCombat::Exit() {
	mSprite.Stop();
	mSprite.Reset();
}

//----------------------------------------------------------------------------------------------------

SRect CharacterCombat::GetBoundingBox() const {
	const SVector2 pos(mpOwner->GetPosition());
	return SRect(pos.x - 16.0f, pos.y - 56.0f, pos.x + 16.0f, pos.y);
}
