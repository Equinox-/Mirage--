#include "Stats.h"

Stats::Stats()
:mVisible(true)
,mPosition(0.0f, 0.0f)
{

}

Stats::~Stats()
{

}

void Stats::Init()
{
	mSprite.Load("GUI/Ingame/Character.png");
}

void Stats::Update(float deltaTime)
{
	mSprite.Update(deltaTime);
}

void Stats::Render()
{
	if(mVisible)
	{
		mSprite.SetPosition(mPosition);
		mSprite.Render();
	}
}
