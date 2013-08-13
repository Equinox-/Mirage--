#include "Player.h"

Player::Player(PlayerInfo playerInfo)
:mPlayerInfo(playerInfo)
,mInit(false)
,mNewPosition(0.0f, 0.0f)
{

}


Player::~Player()
{

}

void Player::Init()
{
	mSprite.Unload();

	int walkAnimations = IniFile_GetInt("SPRITE_WALK_ANIMATIONS", 16);
	char temp[CHAR_MAX];
	for(int a = 1; a < walkAnimations; ++a)
	{
		sprintf(temp,"Sprites/%d/%d.png", mPlayerInfo.GetSpriteNumber(), a);
		mSprite.Add(temp);
	}

	// Get the right frames
	mFrames.WalkDownStart = IniFile_GetInt("WALK_DOWN_START", 1);
	mFrames.WalkDownEnd = IniFile_GetInt("WALK_DOWN_END", 4);
	mFrames.WalkLeftStart = IniFile_GetInt("WALK_LEFT_START", 5);
	mFrames.WalkLeftEnd = IniFile_GetInt("WALK_LEFT_END", 8);
	mFrames.WalkUpStart = IniFile_GetInt("WALK_UP_START", 9);
	mFrames.WalkUpEnd = IniFile_GetInt("WALK_UP_END", 12);

	mInit = true;
	mNewPosition = mPlayerInfo.GetPosition();
}

void Player::Update(float deltaTime)
{
	if(mInit)
	{
		const float kSpeed = 250.0f;
		mSprite.Update(deltaTime);

		// Figure out where we should move
		SVector2 oldPosition = mPlayerInfo.GetPosition();
		SVector2 vel(0.0f, 0.0f);

		Direction direction = mPlayerInfo.GetDirection();;

		bool xDone = false;
		bool yDone = false;

		if(oldPosition.x < mNewPosition.x - 16)
		{
			vel.x += deltaTime * kSpeed;
			//oldPosition.x += mMoveSpeed;
		}
		else if(oldPosition.x > mNewPosition.x + 16)
		{
			vel.x -= deltaTime * kSpeed;
			//oldPosition.x -= mMoveSpeed;
		}
		else
		{
			xDone = true;
		}

		// Move the npc to the next position
		if(oldPosition.y < mNewPosition.y - 16)
		{
			vel.y += deltaTime * kSpeed;
			//oldPosition.y += mMoveSpeed;
		}
		else if(oldPosition.y > mNewPosition.y + 16)
		{
			vel.y -= deltaTime * kSpeed;
			//oldPosition.y -= mMoveSpeed;
		}
		else
		{
			yDone = true;
		}

		switch(direction)
		{
		case right:
			{
				if(mOldDirection != right)
				{
					mAnimation.SetStartFrame(mFrames.WalkLeftStart);
					mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
					mSprite.SetFlipH(true);
					mOldDirection = right;
					mAnimation.SetIsAnimating(true);
				}
				break;
			}

		case left:
			{
				if(mOldDirection != left)
				{
					mAnimation.SetStartFrame(mFrames.WalkLeftStart);
					mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
					mSprite.SetFlipH(false);
					mOldDirection = left;
					mAnimation.SetIsAnimating(true);
				}
				break;
			}

		case up:
			{
				if(mOldDirection != up)
				{
					mAnimation.SetStartFrame(mFrames.WalkUpStart);
					mAnimation.SetEndFrame(mFrames.WalkUpEnd);
					mSprite.SetFlipH(false);
					mOldDirection = up;
					mAnimation.SetIsAnimating(true);
				}
				break;
			}

		case down:
			{
				if(mOldDirection != down)
				{
					mAnimation.SetStartFrame(mFrames.WalkDownStart);
					mAnimation.SetEndFrame(mFrames.WalkDownEnd);
					mSprite.SetFlipH(false);
					mOldDirection = down;
					mAnimation.SetIsAnimating(true);
				}
				break;
			}

		case rightUp:
			{
				if(mOldDirection != rightDown)
				{
					mAnimation.SetStartFrame(mFrames.WalkLeftStart);
					mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
					mSprite.SetFlipH(true);
					mOldDirection = rightDown;
					mAnimation.SetIsAnimating(true);
				}
				break;
			}

		case rightDown:
			{
				if(mOldDirection != rightUp)
				{
					mAnimation.SetStartFrame(mFrames.WalkLeftStart);
					mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
					mSprite.SetFlipH(true);
					mOldDirection = rightUp;
					mAnimation.SetIsAnimating(true);
				}
				break;
			}

		case leftUp:
			{
				if(mOldDirection != leftUp)
				{
					mAnimation.SetStartFrame(mFrames.WalkLeftStart);
					mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
					mSprite.SetFlipH(false);
					mOldDirection = leftUp;
					mAnimation.SetIsAnimating(true);
				}
				break;
			}

		case leftDown:
			{
				if(mOldDirection != leftDown)
				{
					mAnimation.SetStartFrame(mFrames.WalkLeftStart);
					mAnimation.SetEndFrame(mFrames.WalkLeftEnd);
					mSprite.SetFlipH(false);
					mOldDirection = leftDown;
					mAnimation.SetIsAnimating(true);
				}

				break;
			}

		case noDirection:
			{

				break;
			}
		}


		
		if(xDone && yDone)
		{
			mPlayerInfo.SetPosition(mNewPosition);
		}
		else
		{
			mPlayerInfo.SetPosition(oldPosition + vel);
		}
		
		mAnimation.AnimateSprite(deltaTime, mSprite);
		mOldDirection = mPlayerInfo.GetDirection();
		
	}

}

void Player::Render(SVector2& offset)
{
	if(mInit)
	{
		const int kTextureWidth = mSprite.GetWidth();
		const int kTextureHeight = mSprite.GetHeight();
		const SVector2 ownerPos(mPlayerInfo.GetPosition());
		const SVector2 renderPos(ownerPos.x - (kTextureWidth * 0.5f), ownerPos.y - kTextureHeight);

		mSprite.SetPosition(renderPos + offset);
		mSprite.Render();
	}
}