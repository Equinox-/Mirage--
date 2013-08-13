#include "Animation.h"

Animation::Animation() :
		mTimer(0.0f), mFrameTime(0.4f), mCurrentStartFrame(0), mCurrentEndFrame(
				0), mIsAnimating(false) {

}

Animation::~Animation() {

}

void Animation::AnimateSprite(float deltaTime, SGE_Sprite& sprite) {
	if (mIsAnimating) {
		// Update Timer
		mTimer += deltaTime;

		// Cycle Animations
		int frame = sprite.GetCurrentFrame();

		// If we arent already on the right movement frame
		if (frame < mCurrentStartFrame || frame >= mCurrentEndFrame) {
			sprite.SetCurrentFrame(mCurrentStartFrame);
			mTimer = 0;
		}

		if (mTimer > mFrameTime) {
			// Increment frame if enough time has passed
			sprite.SetCurrentFrame(++frame);

			// Reset Time Counter
			mTimer = 0;
		}

		if (frame == mCurrentEndFrame) {
			mIsAnimating = false;
		}
	}
}
