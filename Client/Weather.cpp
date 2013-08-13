#include "Weather.h"

Weather::Weather()
:mCurrentWeather(heavyRain)
{

}

Weather::~Weather()
{

}


void Weather::Init()
{
	mEmitter.Unload();
	switch(mCurrentWeather)
	{
	case rain:
		{
			mEmitter.Load("particle_rain.txt");
			mEmitter.Start(25, false);
			break;
		}

	case heavyRain:
		{
			mEmitter.Load("particle_rain.txt");
			mEmitter.Start(500 , false);
			break;
		}

	case snow:
		{
			mEmitter.Load("particle_snow.txt");
			mEmitter.Start(25, false);
			break;
		}

	case heavySnow:
		{
			mEmitter.Load("particle_snow.txt");
			mEmitter.Start(75, false);
			break;
		}

	case fire:
		{
			mEmitter.Load("particle_flames.txt");
			mEmitter.Start(75, false);
			break;
		}
	case smoke:
		{
			mEmitter.Load("particle_brown.txt");
			mEmitter.Start(50, false);
			break;
		}
	case purple:
		{
			mEmitter.Load("particle_lavenderspot.txt");
			mEmitter.Start(50, false);
			break;
		}
	case ugly:
		{
			mEmitter.Load("particle_ugly.txt");
			mEmitter.Start(2500, false);
			break;
		}
	}

	int width = IniFile_GetInt("WinWidth", 1600);
	int height = IniFile_GetInt("WinHeight", 900);

	mEmitter.SetPosition(0, 0);
	
}

void Weather::Update(float deltaTime)
{
	mEmitter.Update(deltaTime);
}

void Weather::Render(SVector2 offset)
{
	if(Input_IsKeyDown(Keys::LSHIFT))
	{
		mEmitter.SetOffset(offset);
		mEmitter.Render();
	}
}