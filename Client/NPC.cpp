#include "NPC.h"

NPC::NPC(Raknet& raknet, int index) :
		mMapWidth(0), mMapHeight(0), mIndex(index), mReachedDestination(true), mBattling(
				false), mInit(false), mDirection(down), mRaknet(raknet), mGraph(
				NULL), mVelocity(0.0f, 0.0f) {
}

NPC::~NPC() {
	Unload();
}

void NPC::ReloadGraphic() {
	mSprite.Unload();
	int walkAnimations = IniFile_GetInt("SPRITE_WALK_ANIMATIONS", 16);
	char temp[CHAR_MAX];
	for (int a = 1; a < walkAnimations; ++a) {
		sprintf(temp, "Sprites/%d/%d.png", mNPCData.mSpriteNumber, a);
		mSprite.Add(temp);
	}

}

void NPC::Init(int mapWidth, int mapHeight, Graph* graph) {
	mGraph = graph;

	mMapWidth = mapWidth / 32;
	mMapHeight = mapHeight / 32;

	int walkAnimations = IniFile_GetInt("SPRITE_WALK_ANIMATIONS", 16);
	char temp[CHAR_MAX];
	for (int a = 1; a < walkAnimations; ++a) {
		sprintf(temp, "Sprites/%d/%d.png", mNPCData.mSpriteNumber, a);
		mSprite.Add(temp);
	}

	// Get the right frames
	mFrames.WalkDownStart = IniFile_GetInt("WALK_DOWN_START", 1);
	mFrames.WalkDownEnd = IniFile_GetInt("WALK_DOWN_END", 4);
	mFrames.WalkLeftStart = IniFile_GetInt("WALK_LEFT_START", 5);
	mFrames.WalkLeftEnd = IniFile_GetInt("WALK_LEFT_END", 8);
	mFrames.WalkUpStart = IniFile_GetInt("WALK_UP_START", 9);
	mFrames.WalkUpEnd = IniFile_GetInt("WALK_UP_END", 12);

	mFont.SetColor(255, 0, 255);
	mInit = true;
}

void NPC::Update(float deltaTime, Map* map, Character& character) {
	int x = (RoundUp((int) mNPCData.mPosition.x + (mSprite.GetWidth() * 0.5),
			32) / 32) - 1;
	int y = (RoundUp((int) mNPCData.mPosition.y + (mSprite.GetHeight() * 0.5),
			32) / 32) - 1;
	int index = x + (y * mMapWidth);

	if (!mInit || !mNPCData.mCanMove)
		return;

	int tiles = mMapWidth * mMapHeight;

	// If we have reached our destination and are not fighting find a new place to walk
	if (mReachedDestination && !mBattling) {
		if (mNPCData.mHost) {
			mNPCData.mPathEnd = rand() % tiles;

			while (!map->GetWalkable(mNPCData.mPathEnd)) {
				mNPCData.mPathEnd = rand() % tiles;
			}

			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID) ID_NPC_NEW_PATH);

			// Npc Array Slot
			bsOut.Write(mIndex);

			// End Position index
			bsOut.Write(mNPCData.mPathEnd);

			bsOut.Write(mNPCData.mPosition);

			mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
					mRaknet.mServerAddress, false);
		}
	} else if (!mNPCData.mStop) {
		SVector2 oldPosition = mNPCData.mPosition;
		mVelocity = SVector2(0.0f, 0.0f);
		// Smooths out Animation jitters
		for (int a = 0; a < 3; ++a) {
			// Move us towards are location
			if (mNPCData.mPathIndex < mPath.size()) {
				bool xDone = false;
				SVector2 position = mPath[mNPCData.mPathIndex];

				CheckCollision(deltaTime, *map, character);
				if (mNPCData.mPosition.x < position.x - mNPCData.mMoveSpeed) {
					mVelocity.x += mNPCData.mMoveSpeed;
				} else if (mNPCData.mPosition.x
						> position.x + mNPCData.mMoveSpeed) {
					mVelocity.x -= mNPCData.mMoveSpeed;
				} else {
					// We are at the right x location
					xDone = true;
				}

				// Move the npc to the next position
				if (mNPCData.mPosition.y < position.y - mNPCData.mMoveSpeed) {
					mVelocity.y += mNPCData.mMoveSpeed;
				} else if (mNPCData.mPosition.y
						> position.y + mNPCData.mMoveSpeed) {
					mVelocity.y -= mNPCData.mMoveSpeed;
				} else {
					// We are at the next tile
					if (xDone) {
						if (mNPCData.mHost) {
							RakNet::BitStream bsOut;
							bsOut.Write((RakNet::MessageID) ID_NPC_POSITION);
							bsOut.Write(mIndex); // Array location
							bsOut.Write(mNPCData.mPathIndex);
							bsOut.Write(mNPCData.mPosition);
							mRaknet.mPeer->Send(&bsOut, HIGH_PRIORITY,
									RELIABLE_ORDERED, 0, mRaknet.mServerAddress,
									false);
						}

						++mNPCData.mPathIndex;
					}
				}
			} else {
				mReachedDestination = true;
			}
		}

		Direction oldDirection = mDirection;
		SVector2 positionDifferance = mNPCData.mPosition - oldPosition;

		// Right
		if (positionDifferance.x > 0 && positionDifferance.y == 0) {
			if (oldDirection != right) {
				mAnimation.SetStartFrame(mFrames.WalkLeftStart);
				mAnimation.SetEndFrame(mFrames.WalkLeftEnd);

				mSprite.SetFlipH(true);
				mDirection = right;
			}
		}
		// Left
		else if (positionDifferance.x < 0 && positionDifferance.y == 0) {
			if (oldDirection != left) {
				mAnimation.SetStartFrame(mFrames.WalkLeftStart);
				mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
				mSprite.SetFlipH(false);
				mDirection = left;
			}
		}
		// Up
		else if (positionDifferance.y > 0 && positionDifferance.x == 0) {
			if (oldDirection != up) {
				mAnimation.SetStartFrame(mFrames.WalkDownStart);
				mAnimation.SetEndFrame(mFrames.WalkDownEnd);

				mSprite.SetFlipH(false);
				mDirection = up;
			}
		}
		// Down
		else if (positionDifferance.y < 0 && positionDifferance.x == 0) {
			if (oldDirection != down) {
				mAnimation.SetStartFrame(mFrames.WalkUpStart);
				mAnimation.SetEndFrame(mFrames.WalkUpEnd);

				mSprite.SetFlipH(false);
				mDirection = down;
			}
		}

		// Up Right
		else if (positionDifferance.x > 0 && positionDifferance.y < 0) {
			if (oldDirection != rightDown) {
				mAnimation.SetStartFrame(mFrames.WalkLeftStart);
				mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
				mSprite.SetFlipH(true);
				mDirection = rightDown;
			}
		}
		// Down Right
		else if (positionDifferance.x > 0 && positionDifferance.y > 0) {
			if (oldDirection != rightUp) {
				mAnimation.SetStartFrame(mFrames.WalkLeftStart);
				mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
				mSprite.SetFlipH(true);
				mDirection = rightUp;
			}
		}

		// Up Left
		else if (positionDifferance.x < 0 && positionDifferance.y > 0) {
			if (oldDirection != leftUp) {
				mAnimation.SetStartFrame(mFrames.WalkLeftStart);
				mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
				mSprite.SetFlipH(false);
				mDirection = leftUp;
			}
		}
		// Down Left
		else if (positionDifferance.x < 0 && positionDifferance.y < 0) {
			if (oldDirection != leftDown) {
				mAnimation.SetStartFrame(mFrames.WalkLeftStart);
				mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
				mSprite.SetFlipH(false);
				mDirection = leftDown;
			}
		}

		if (positionDifferance.x != 0 || positionDifferance.y != 0) {
			mSprite.Update(deltaTime);
			mAnimation.AnimateSprite(deltaTime, mSprite);
			mAnimation.SetIsAnimating(true);
		}
	}

	if (GetStop()) {
		CheckCollision(deltaTime, *map, character);
	}
}

static SVector2 sOffset;
void NPC::Render(SVector2 offset) {
	sOffset = offset;

	SVector2 newPos = (mNPCData.mPosition + offset);
	mSprite.SetPosition(newPos);
	mSprite.Render();

	mFont.Print(mNPCData.mName, mNPCData.mPosition.x + offset.x,
			mNPCData.mPosition.y + offset.y - 15);

#if _DEBUG

	int size = mPath.size() - 1;

	if(Input_IsKeyDown(Keys::TAB))
	{

		for(int a = 0; a < size; ++a)
		{
			SVector2 newPosA = (mPath[a] + offset);
			SVector2 newPosB = (mPath[a + 1] + offset);
			Graphics_DebugLine(newPosA, newPosB, 0XFF0000);
		}
	}

#endif
}

void NPC::Unload() {
	mFont.Unload();
}

void NPC::CreatePath() {
	mNPCData.mPathIndex = 0;
	int x = (RoundUp((int) mNPCData.mPosition.x, 32) / 32);
	int y = (RoundUp((int) mNPCData.mPosition.y, 32) / 32);
	int startIndex = x + (y * mMapWidth);

	WalkableFunctor walkF = WalkableFunctor();
	ICostFunctor costF = ICostFunctor();
	IHeuristicFunctor heuristicF = IHeuristicFunctor();
	mGraph->SearchAStar(startIndex, mNPCData.mPathEnd, walkF, costF,
			heuristicF);

	NodeList nodeList = mGraph->GetPath();
	mPath.clear();

	for (NodeList::iterator iter = nodeList.begin(); iter != nodeList.end();
			++iter) {
		mPath.push_back((*iter)->GetPosition());
	}

	mReachedDestination = false;
}

SRect NPC::GetBoundingBox() const {
	return SRect(mNPCData.mPosition.x + 5, mNPCData.mPosition.y,
			mNPCData.mPosition.x + 25, mNPCData.mPosition.y + 50);
}

void NPC::CheckCollision(float fSeconds, Map& map, Character& character) {
	// Check collision
	SRect bb = GetBoundingBox();
	SRect newbb = bb + SVector2(mVelocity.x, 0.0f);
	SRect rightbb = map.GetBoundingBoxFromSegment(newbb.GetRightSegment(),
			*this, character);
	SRect leftbb = map.GetBoundingBoxFromSegment(newbb.GetLeftSegment(), *this,
			character);

	// Right collision
	if (mVelocity.x > 0.0f && rightbb.IsValid()) {
		mNPCData.mPosition.x += static_cast<int>(rightbb.min.x - bb.max.x)
				- 1.0f;
	}
	// Left collision
	else if (mVelocity.x < 0.0f && leftbb.IsValid()) {
		mNPCData.mPosition.x += static_cast<int>(leftbb.max.x - bb.min.x)
				+ 1.0f;
	} else {
		mNPCData.mPosition.x += static_cast<int>(mVelocity.x);
	}

	// Check collision
	newbb = bb + SVector2(0.0f, mVelocity.y);
	SRect bottombb = map.GetBoundingBoxFromSegment(newbb.GetBottomSegment(),
			*this, character);
	SRect topbb = map.GetBoundingBoxFromSegment(newbb.GetTopSegment(), *this,
			character);

	// Bottom collision
	if (mVelocity.y > 0.0f && bottombb.IsValid()) {
		mNPCData.mPosition.y += static_cast<int>(bottombb.min.y - bb.max.y)
				- 1.0f;
		mVelocity.y = 0.0f;
		Graphics_DebugRect(newbb + sOffset, 0xff00ff);
		Graphics_DebugRect(bottombb + sOffset, 0xff0000);
	}

	// Top collision
	else if (mVelocity.y < 0.0f && topbb.IsValid()) {
		mNPCData.mPosition.y += static_cast<int>(topbb.max.y - bb.min.y) + 1.0f;
		mVelocity.y = 0.0f;
		Graphics_DebugRect(newbb + sOffset, 0xff00ff);
		Graphics_DebugRect(topbb + sOffset, 0xff0000);
	} else {
		mNPCData.mPosition.y += static_cast<int>(mVelocity.y);
	}
}
