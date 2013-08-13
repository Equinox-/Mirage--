#ifndef INCLUDED_RANDOM_H
#define INCLUDED_RANDOM_H

//====================================================================================================
// Filename:	Random.h
// Created by:	Peter Chan
// Description:	Header containing some useful random functions. A call to Random::SetRandSeed() can be
//			  called during program initialization to set the random seed. If no seed is passed,
//			  time will be used as the random seed.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <stdlib.h>
#include <time.h>
#include "../GLUtil/Math/Vector2f.h"
#include "../GLUtil/Math/Vector3f.h"
#include "../GLUtil/GLColor.h"

//====================================================================================================
// Namespace
//====================================================================================================

namespace Random
{

//====================================================================================================
// Function Declarations
//====================================================================================================

// Function to set random seed
inline void SetRandSeed(int iSeed = -1);

// Function to return a random int
inline int GetInt(int iLower, int iUpper);

// Function to return a random float
inline float GetFloat(float fLower, float fUpper);

// Functions to return a random vector
inline Vector2f GetVector2(const Vector2f& v0, const Vector2f& v1);
inline Vector3f GetVector3(const Vector3f& v0, const Vector3f& v1);

// Function to return a random color
inline GLColor GetColor(const GLColor& c0, const GLColor& c1);

//====================================================================================================
// Function Definitions
//====================================================================================================

inline void SetRandSeed(int iSeed)
{
	// Check if a seed is provided
	if (-1 == iSeed)
	{
		// Set random seed with time
		srand((unsigned int)time(0));
	}
	else
	{
		// Use the random seed provided
		srand(iSeed);
	}
}

//----------------------------------------------------------------------------------------------------

inline int GetInt(int iLower, int iUpper)
{
	// Create a random float from 0.0f to 1.0f
	float fRandFloat = rand() / static_cast<float>(RAND_MAX);

	// Return a number between iLower and iUpper
	return iLower + static_cast<int>((iUpper - iLower) * fRandFloat);
}

//----------------------------------------------------------------------------------------------------

inline float GetFloat(float fLower, float fUpper)
{
	// Create a random float from 0.0f to 1.0f
	float fRandFloat = rand() / static_cast<float>(RAND_MAX);

	// Return a number between fLower and fUpper
	return fLower + ((fUpper - fLower) * fRandFloat);
}

//----------------------------------------------------------------------------------------------------

inline Vector2f GetVector2(const Vector2f& v0, const Vector2f& v1)
{
	Vector2f returnMe;

	// Create a random vector
	returnMe.x = GetFloat(v0.x, v1.x);
	returnMe.y = GetFloat(v0.y, v1.y);

	// Return new vector
	return returnMe;
}

//----------------------------------------------------------------------------------------------------

inline Vector3f GetVector3(const Vector3f& v0, const Vector3f& v1)
{
	Vector3f returnMe;

	// Create a random vector
	returnMe.x = GetFloat(v0.x, v1.x);
	returnMe.y = GetFloat(v0.y, v1.y);
	returnMe.z = GetFloat(v0.z, v1.z);

	// Return new vector
	return returnMe;
}

//----------------------------------------------------------------------------------------------------

inline GLColor GetColor(const GLColor& c0, const GLColor& c1)
{
	GLColor returnMe;

	// Create a random color
	returnMe.r = GetFloat(c0.r, c1.r);
	returnMe.g = GetFloat(c0.g, c1.g);
	returnMe.b = GetFloat(c0.b, c1.b);
	returnMe.a = GetFloat(c0.a, c1.a);

	// Return new color
	return returnMe;
}

} // namespace Random

#endif // #ifndef INCLUDED_RANDOM_H
