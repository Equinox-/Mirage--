#include "InputBox.h"

InputBox::InputBox(float x, float y, int width, int height, bool password, bool visible, float r, float g, float b, float spacing, bool bold, int maxLength)
	:mPosition(x, y)
	,mWidth(width)
	,mHeight(height)
	,mActive(false)
	,mTextPosition(0)
	,mMaxLength(maxLength)
	,mIsPassword(password)
	,mIsVisible(visible)
	,mR(r)
	,mG(g)
	,mB(b)
	,mSpacing(spacing)
	,mBold(bold)
{
	int length = maxLength + 1;
	mText = new char[length];
	memset(mText, '\0', length);
}

InputBox::~InputBox()
{
	mSprite.Unload();
	mFont.Unload();

	if(mText)
	{
		delete [] mText;
		mText = NULL;
	}
}

void InputBox::Update(float deltaTime)
{
	mSprite.Update(deltaTime);

	SVector2 mouse((float) Input_GetMouseScreenX(), (float) Input_GetMouseScreenY());
	// If a mouse click is detected this box is no longer selected. Below it checks if it is the one being clicked on
	if(Input_IsMousePressed(Mouse::LBUTTON))
	{
		mActive = mouse.x > (mPosition.x - 25) && mouse.x < mPosition.x + mWidth && mouse.y > (mPosition.y - 25) && mouse.y < mPosition.y + mHeight;
	}

	// If Active Capture Text
	if(mActive)
	{
		// Dont go over max length
		if(mTextPosition < mMaxLength)
		{		if (Input_IsKeyDown(Keys::LSHIFT) || Input_IsKeyDown(Keys::RSHIFT) )
				{
					if(Input_IsKeyPressed(Keys::A)) { mText[mTextPosition++] = 'A'; }
					if(Input_IsKeyPressed(Keys::B)) { mText[mTextPosition++] = 'B'; }
					if(Input_IsKeyPressed(Keys::C)) { mText[mTextPosition++] = 'C'; }
					if(Input_IsKeyPressed(Keys::D)) { mText[mTextPosition++] = 'D'; }
					if(Input_IsKeyPressed(Keys::E)) { mText[mTextPosition++] = 'E'; }
					if(Input_IsKeyPressed(Keys::F)) { mText[mTextPosition++] = 'F'; }
					if(Input_IsKeyPressed(Keys::G)) { mText[mTextPosition++] = 'G'; }
					if(Input_IsKeyPressed(Keys::H)) { mText[mTextPosition++] = 'H'; }
					if(Input_IsKeyPressed(Keys::I)) { mText[mTextPosition++] = 'I'; }
					if(Input_IsKeyPressed(Keys::J)) { mText[mTextPosition++] = 'J'; }
					if(Input_IsKeyPressed(Keys::K)) { mText[mTextPosition++] = 'K'; }
					if(Input_IsKeyPressed(Keys::L)) { mText[mTextPosition++] = 'L'; }
					if(Input_IsKeyPressed(Keys::M)) { mText[mTextPosition++] = 'M'; }
					if(Input_IsKeyPressed(Keys::N)) { mText[mTextPosition++] = 'N'; }
					if(Input_IsKeyPressed(Keys::O)) { mText[mTextPosition++] = 'O'; }
					if(Input_IsKeyPressed(Keys::P)) { mText[mTextPosition++] = 'P'; }
					if(Input_IsKeyPressed(Keys::Q)) { mText[mTextPosition++] = 'Q'; }
					if(Input_IsKeyPressed(Keys::R)) { mText[mTextPosition++] = 'R'; }
					if(Input_IsKeyPressed(Keys::S)) { mText[mTextPosition++] = 'S'; }
					if(Input_IsKeyPressed(Keys::T)) { mText[mTextPosition++] = 'T'; }
					if(Input_IsKeyPressed(Keys::U)) { mText[mTextPosition++] = 'U'; }
					if(Input_IsKeyPressed(Keys::V)) { mText[mTextPosition++] = 'V'; }
					if(Input_IsKeyPressed(Keys::W)) { mText[mTextPosition++] = 'W'; }
					if(Input_IsKeyPressed(Keys::X)) { mText[mTextPosition++] = 'X'; }
					if(Input_IsKeyPressed(Keys::Y)) { mText[mTextPosition++] = 'Y'; }
					if(Input_IsKeyPressed(Keys::Z)) { mText[mTextPosition++] = 'Z'; }

					if(Input_IsKeyPressed(Keys::SEMICOLON)) { mText[mTextPosition++] = ':'; }
					if(Input_IsKeyPressed(Keys::ONE)) { mText[mTextPosition++] = '!'; }
					if(Input_IsKeyPressed(Keys::TWO)) { mText[mTextPosition++] = '@'; }
					if(Input_IsKeyPressed(Keys::THREE)) { mText[mTextPosition++] = '#'; }
					if(Input_IsKeyPressed(Keys::FOUR)) { mText[mTextPosition++] = '$'; }
					if(Input_IsKeyPressed(Keys::FIVE)) { mText[mTextPosition++] = '%'; }
					if(Input_IsKeyPressed(Keys::SIX)) { mText[mTextPosition++] = '^'; }
					if(Input_IsKeyPressed(Keys::SEVEN)) { mText[mTextPosition++] = '&'; }
					if(Input_IsKeyPressed(Keys::EIGHT)) { mText[mTextPosition++] = '*'; }
					if(Input_IsKeyPressed(Keys::NINE)) { mText[mTextPosition++] = '('; }
					if(Input_IsKeyPressed(Keys::ZERO)) { mText[mTextPosition++] = ')'; }
					if(Input_IsKeyPressed(Keys::SLASH)) { mText[mTextPosition++] = '?'; }
					if(Input_IsKeyPressed(Keys::PERIOD)) { mText[mTextPosition++] = '>'; }
					if(Input_IsKeyPressed(Keys::COMMA)) { mText[mTextPosition++] = '<'; }
					if(Input_IsKeyPressed(Keys::APOSTROPHE)) { mText[mTextPosition++] = '"'; }
					if(Input_IsKeyPressed(Keys::GRAVE)) { mText[mTextPosition++] = '~'; }
					if(Input_IsKeyPressed(Keys::LBRACKET)) { mText[mTextPosition++] = '{'; }
					if(Input_IsKeyPressed(Keys::RBRACKET)) { mText[mTextPosition++] = '}'; }
				}
				else
				{
					if(Input_IsKeyPressed(Keys::A)) { mText[mTextPosition++] = 'a'; }
					if(Input_IsKeyPressed(Keys::B)) { mText[mTextPosition++] = 'b'; }
					if(Input_IsKeyPressed(Keys::C)) { mText[mTextPosition++] = 'c'; }
					if(Input_IsKeyPressed(Keys::D)) { mText[mTextPosition++] = 'd'; }
					if(Input_IsKeyPressed(Keys::E)) { mText[mTextPosition++] = 'e'; }
					if(Input_IsKeyPressed(Keys::F)) { mText[mTextPosition++] = 'f'; }
					if(Input_IsKeyPressed(Keys::G)) { mText[mTextPosition++] = 'g'; }
					if(Input_IsKeyPressed(Keys::H)) { mText[mTextPosition++] = 'h'; }
					if(Input_IsKeyPressed(Keys::I)) { mText[mTextPosition++] = 'i'; }
					if(Input_IsKeyPressed(Keys::J)) { mText[mTextPosition++] = 'j'; }
					if(Input_IsKeyPressed(Keys::K)) { mText[mTextPosition++] = 'k'; }
					if(Input_IsKeyPressed(Keys::L)) { mText[mTextPosition++] = 'l'; }
					if(Input_IsKeyPressed(Keys::M)) { mText[mTextPosition++] = 'm'; }
					if(Input_IsKeyPressed(Keys::N)) { mText[mTextPosition++] = 'n'; }
					if(Input_IsKeyPressed(Keys::O)) { mText[mTextPosition++] = 'o'; }
					if(Input_IsKeyPressed(Keys::P)) { mText[mTextPosition++] = 'p'; }
					if(Input_IsKeyPressed(Keys::Q)) { mText[mTextPosition++] = 'q'; }
					if(Input_IsKeyPressed(Keys::R)) { mText[mTextPosition++] = 'r'; }
					if(Input_IsKeyPressed(Keys::S)) { mText[mTextPosition++] = 's'; }
					if(Input_IsKeyPressed(Keys::T)) { mText[mTextPosition++] = 't'; }
					if(Input_IsKeyPressed(Keys::U)) { mText[mTextPosition++] = 'u'; }
					if(Input_IsKeyPressed(Keys::V)) { mText[mTextPosition++] = 'v'; }
					if(Input_IsKeyPressed(Keys::W)) { mText[mTextPosition++] = 'w'; }
					if(Input_IsKeyPressed(Keys::X)) { mText[mTextPosition++] = 'x'; }
					if(Input_IsKeyPressed(Keys::Y)) { mText[mTextPosition++] = 'y'; }
					if(Input_IsKeyPressed(Keys::Z)) { mText[mTextPosition++] = 'z'; }

					if(Input_IsKeyPressed(Keys::ZERO)) { mText[mTextPosition++] = '0'; }
					if(Input_IsKeyPressed(Keys::ONE)) { mText[mTextPosition++] = '1'; }
					if(Input_IsKeyPressed(Keys::TWO)) { mText[mTextPosition++] = '2'; }
					if(Input_IsKeyPressed(Keys::THREE)) { mText[mTextPosition++] = '3'; }
					if(Input_IsKeyPressed(Keys::FOUR)) { mText[mTextPosition++] = '4'; }
					if(Input_IsKeyPressed(Keys::FIVE)) { mText[mTextPosition++] = '5'; }
					if(Input_IsKeyPressed(Keys::SIX)) { mText[mTextPosition++] = '6'; }
					if(Input_IsKeyPressed(Keys::SEVEN)) { mText[mTextPosition++] = '7'; }
					if(Input_IsKeyPressed(Keys::EIGHT)) { mText[mTextPosition++] = '8'; }
					if(Input_IsKeyPressed(Keys::NINE)) { mText[mTextPosition++] = '9'; }
					if(Input_IsKeyPressed(Keys::SLASH)) { mText[mTextPosition++] = '/'; }
					if(Input_IsKeyPressed(Keys::PERIOD)) { mText[mTextPosition++] = '.'; }
					if(Input_IsKeyPressed(Keys::APOSTROPHE)) { mText[mTextPosition++] = '\''; }
					if(Input_IsKeyPressed(Keys::COMMA)) { mText[mTextPosition++] = ','; }
					if(Input_IsKeyPressed(Keys::MINUS)) { mText[mTextPosition++] = '-'; }
					if(Input_IsKeyPressed(Keys::EQUALS)) { mText[mTextPosition++] = '='; }
					if(Input_IsKeyPressed(Keys::SEMICOLON)) { mText[mTextPosition++] = ';'; }
					if(Input_IsKeyPressed(Keys::GRAVE)) { mText[mTextPosition++] = '`'; }
					if(Input_IsKeyPressed(Keys::LBRACKET)) { mText[mTextPosition++] = '['; }
					if(Input_IsKeyPressed(Keys::RBRACKET)) { mText[mTextPosition++] = ']'; }
				}

				if(Input_IsKeyPressed(Keys::SPACE)) { mText[mTextPosition++] = ' '; }
		}

		// Backspace
		if(mTextPosition > 0 && Input_IsKeyPressed(Keys::BACKSPACE))
		{
			mText[--mTextPosition] = '\0';
		}
	}
}

void InputBox::Render()
{
	if(mIsVisible)
	{
		mSprite.Render();
	}

	if(!mIsPassword)
	{
		mFont.Print(mText, (int)(mPosition.x + mSpacing), (int)(mPosition.y + 2.0f));
	}
	else
	{
		char temp[CHAR_MAX];
		memset(temp, '\n', CHAR_MAX);
		memset(temp, '*', mTextPosition);
		mFont.Print(temp, (int)(mPosition.x + mSpacing), (int)(mPosition.y + 2.0f));
	}
}

void InputBox::Init()
{
	mFont.Load(14, mBold);
	mFont.SetColor(mR,mG,mB);
	mSprite.Add("Input.png");
	mSprite.SetPosition(mPosition);
}
