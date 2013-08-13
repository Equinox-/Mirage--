#include "DragSprite.h"

DragSprite::DragSprite(SVector2 position, bool visible)
:mPosition(position)
,mVisible(visible)
,mDragDown(false)
{

}
DragSprite::~DragSprite()
{

}

void DragSprite::Init(char* fileName)
{
	mSprite.Load(fileName);
	mSprite.SetPosition(mPosition);
}

void DragSprite::Update(float deltaTime)
{
	if(mVisible)
	{
		float x = Input_GetMouseScreenX();
		float y = Input_GetMouseScreenY();

		if(Input_IsMouseDown(Mouse::LBUTTON) && x - mPosition.x > 176 && y  - mPosition.y < 16)
		{
			mVisible = false;
		}

		// Draggable Inventory
		else if(Input_IsMouseDown(Mouse::LBUTTON) && 
			(x > mPosition.x && x < mPosition.x + mSprite.GetWidth() - 20)
			&& (y > mPosition.y && y < mPosition.y + 30))
		{
			mDragDown = true;
		}

		if(!Input_IsMouseDown(Mouse::LBUTTON) && mDragDown)
		{	
			mDragDown = false;
		}

		if(mDragDown)
		{
			mPosition = SVector2(x - 75, y - 20);
			mSprite.Update(deltaTime);
		}
	}
}

void DragSprite::Render()
{
	if(mVisible)
	{
		mSprite.SetPosition(mPosition);
		mSprite.Render();
	}
}