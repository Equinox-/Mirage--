#ifndef _INPUTBOX_H
#define _INPUTBOX_H

#include <SGE.h>
using namespace SGE;
#include <string.h>
#include <limits.h>

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class InputBox {
public:
	InputBox(float x, float y, int width, int height, bool password = false,
			bool visible = true, float r = 0.0, float g = 0.0, float b = 0.0,
			float spacing = 3.0f, bool bold = false, int maxLength = CHAR_MAX);
	~InputBox();

	void Init();
	void Update(float deltaTime);
	void Render();
	char* GetString() {
		return mText;
	}
	int GetSize() {
		return mTextPosition;
	}

	void SetActive(bool truefalse) {
		mActive = truefalse;
	}
	void ClearText() {
		memset(mText, '\0', CHAR_MAX);
		mTextPosition = 0;
	}
	void SetMaxLength(int length) {
		mMaxLength = length;
	}

private:

	SVector2 mPosition;
	int mWidth;
	int mHeight;
	bool mActive;
	char* mText;
	char* mTempText;
	int mTextPosition;
	int mMaxLength;
	bool mIsVisible;
	bool mIsPassword;

	SGE_Sprite mSprite;
	SGE_Font mFont;

	float mR, mG, mB;
	float mSpacing;
	bool mBold;
	float mCaret;
};

#endif
