//====================================================================================================
// Includes
//====================================================================================================

#include <SGE.h>
using namespace SGE;

//====================================================================================================
// Globals
//====================================================================================================

bool gQuit = false;
SGE_Emitter gEmmiter;
SGE_Font gFont;


//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

void SGE_Initialize()
{
	gEmmiter.Load("../../Particle Editor/particle.txt");
	gEmmiter.SetPosition(320,240);
	gEmmiter.Start(500, false);
	gFont.Load(20, true);
}

void SGE_Terminate()
{

}

bool SGE_Update(float deltaTime)
{
	//mEmmiter.Start();
	gEmmiter.Update(deltaTime);

	if(Input_IsKeyPressed(Keys::ENTER))
	{
		gEmmiter.Load("../../Particle Editor/particle.txt");
		gEmmiter.Start(50, false);
	}

	if(Input_IsKeyPressed(Keys::ESCAPE))
	{
		gQuit = true;
	}

	return gQuit;
}

void SGE_Render()
{
	gFont.Print("Press Enter to reload Particle", 5, 0);
	gEmmiter.Render();
}
