#include "Character.h"

Character::Character(Map& map, PlayerInfo& playerInfo, Raknet& raknet) :
		mMap(map), mPosition(0.0f, 0.0f), mVelocity(0.0f, 0.0f), mDirection(
				left), mFrames(56), mIdleTimer(0), mJumping(true), mFacingLeft(
				false), mpCurrentState(NULL), mPlayerInfo(playerInfo), mInit(
				false), mRaknet(raknet), mUpdateServerTimer(0), mPlayerPositionTime(
				0.0f), mUpdateResult(false), mOnItem(false), mOnItemIndex(0), mUsernameLength(
				0), mTileIndex(0), mFont() {
	memset(mCharacterStates, 0, sizeof(CharacterState*) * AS_Max);
}

void Character::Load(void) {
	mCharacterStates[AS_Walk] = new CharacterWalk(this, mMap, mPlayerInfo,
			mRaknet);
	mCharacterStates[AS_Idle] = new CharacterIdle(this, mPlayerInfo, mRaknet);
	mCharacterStates[AS_Combat] = new CharacterCombat(this, mPlayerInfo,
			mRaknet);

	// Load state resources
	for (int i = 0; i < AS_Max; ++i) {
		mCharacterStates[i]->Load();
	}

	// Set default state
	ChangeState(AS_Walk);

	// Set our position
	mPosition = mPlayerInfo.GetPosition();
	mInit = true;

	mFont.Load(17, true);
	mFont.SetColor(255, 255, 255);

	mPlayerPositionTime = IniFile_GetFloat("PLAYER_POSITION_TIMER", 0.1f);
	mUsernameLength = mPlayerInfo.GetUsernameLength();
}
void Character::Unload(void) {
	//mSprite.Unload();

	for (int i = 0; i < AS_Max; ++i) {
		if (mCharacterStates[i] != NULL) {
			mCharacterStates[i]->Unload();
			delete mCharacterStates[i];
			mCharacterStates[i] = NULL;
		}
	}
	mpCurrentState = NULL;

}
void Character::Update(float deltaTime) {
	if (mInit) {
		mUpdateServerTimer += deltaTime;

		// Send our position to server if enough time has passed
		if ((mUpdateServerTimer > mPlayerPositionTime) && mUpdateResult) {
			mPlayerInfo.SetPosition(mPosition);
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID) ID_UPDATE_POSITION);
			bsOut.Write(mPlayerInfo.GetPosition());
			bsOut.Write(mPlayerInfo.GetDirection());
			mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
					mRaknet.mServerAddress, false);

			mUpdateServerTimer = 0;
			mUpdateResult = false;
		}

		mIdleTimer += deltaTime;
		const float kSpeed = 200.0f;

		// Update the current state
		bool moved = mpCurrentState->Update(deltaTime);

		//Update if we moved
		if (moved) {
			mUpdateResult = true;
		}

		// Apply movement
		CheckCollision(deltaTime, mMap);
	}
}

void Character::Render(const SVector2& offset) {
	if (mInit) {
		mpCurrentState->Render(offset);
	}
}

SRect Character::GetBoundingBox() const {
	return SRect(mPosition.x - 10.0f, mPosition.y - 5.0f, mPosition.x + 11.0f,
			mPosition.y + 0.0f);
}

Character::~Character(void) {

}

void Character::ChangeState(AnimationState as) {
	// Check if we are already in this state
	if (mpCurrentState == mCharacterStates[as]) {
		return;
	}

	// Exit the current state
	if (mpCurrentState != NULL) {
		mpCurrentState->Exit();
	}

	// Switch to the new state
	mpCurrentState = mCharacterStates[as];

	// Enter the new state
	if (mpCurrentState != NULL) {
		mpCurrentState->Enter();
	}
}

//----------------------------------------------------------------------------------------------------

const char* Character::GetCurrentState() {
	const char* name = "Unknown";
	if (mpCurrentState != NULL) {
		name = mpCurrentState->GetName();
	}
	return name;
}

void Character::CheckCollision(float fSeconds, Map& map) {
	// Check collision
	SRect bb = GetBoundingBox();
	SRect newbb = bb + SVector2(mVelocity.x, 0.0f);
	SRect rightbb = map.GetBoundingBoxFromSegment(newbb.GetRightSegment(),
			*(this));
	SRect leftbb = map.GetBoundingBoxFromSegment(newbb.GetLeftSegment(),
			*(this));

	// Right collision
	if (mVelocity.x > 0.0f && rightbb.IsValid()) {
		mPosition.x += static_cast<int>(rightbb.min.x - bb.max.x) - 1.0f;
	}
	// Left collision
	else if (mVelocity.x < 0.0f && leftbb.IsValid()) {
		mPosition.x += static_cast<int>(leftbb.max.x - bb.min.x) + 1.0f;
	} else {
		mPosition.x += static_cast<int>(mVelocity.x);
	}

	// Check collision
	newbb = bb + SVector2(0.0f, mVelocity.y);
	SRect bottombb = map.GetBoundingBoxFromSegment(newbb.GetBottomSegment(),
			*this);
	SRect topbb = map.GetBoundingBoxFromSegment(newbb.GetTopSegment(), *this);

	// Bottom collision
	if (mVelocity.y > 0.0f && bottombb.IsValid()) {
		mPosition.y += static_cast<int>(bottombb.min.y - bb.max.y) - 1.0f;
		mVelocity.y = 0.0f;
	}

	// Top collision
	else if (mVelocity.y < 0.0f && topbb.IsValid()) {
		mPosition.y += static_cast<int>(topbb.max.y - bb.min.y) + 1.0f;
		mVelocity.y = 0.0f;
	} else {
		mPosition.y += static_cast<int>(mVelocity.y);
	}
}

void NewPlayerSprite(int num) {

}
