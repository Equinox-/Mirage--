#include "DrawBar.h"

DrawBar::DrawBar(float length)
:mLength(length)
{

}

DrawBar::~DrawBar()
{

}


void DrawBar::Init(float x, float y,  char* fileName)
{
	mSpriteCurrent.Load(fileName);
	mSpriteCurrent.SetPosition(x,y);
}

void DrawBar::Update(float deltaTime, float currentValue, float totalValue)
{
	float calc = (currentValue / totalValue) * mLength;

	if(calc >= 0)
	{
		mSpriteCurrent.SetScale( calc, 1.0f, false);
	}
}

void DrawBar::Render()
{
	mSpriteCurrent.Render();
}
