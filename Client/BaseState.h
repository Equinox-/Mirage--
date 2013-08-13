#pragma once

#ifndef INCLUDED_BASESTATE_H
#define INCLUDED_BASESTATE_H

#include "Raknet.h"

//====================================================================================================
// Includes
//====================================================================================================

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================


class BaseState
{
public:

	BaseState(Raknet& rak);	
	virtual ~BaseState();		

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual int Update(float deltaTime) = 0;
	virtual void Render() = 0;

protected:
	Raknet& mRaknet;
};
#endif