#ifndef INCLUDED_SGE_MATH_H
#define INCLUDED_SGE_MATH_H

//====================================================================================================
// Filename:	SGE_Math.h
// Created by:	Peter Chan
// Description:	Header containing some useful math structures and helper functions.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <float.h>
#include <math.h>

#ifdef min
	#undef min
#endif

#ifdef max
	#undef max
#endif

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// Constants
//====================================================================================================

static const float kPI		= 3.1415926535f;
static const float kEpsilon	= 0.000001f;

//====================================================================================================
// Structs
//====================================================================================================

struct SVector2
{
	float x;
	float y;

	// Constructors
	SVector2(void);
	SVector2(float _x, float _y);
	SVector2(const SVector2& v);

	// Helper functions
	void Invert(void);
	void PerpendicularLH(void);
	void PerpendicularRH(void);
	float Length(void) const;
	float LengthSquared(void) const;
	void Normalize(void);
	void Truncate(float max);
	void Rotate(float rad);
	void Reflect(const SVector2& normal);

	// Overloaded operators
	inline SVector2 operator+(const SVector2& v) const
	{
		return SVector2(x + v.x, y + v.y);
	}
	SVector2 operator-(const SVector2& v) const;
	SVector2 operator*(float f) const;
	SVector2 operator/(float f) const;

	SVector2& operator+=(const SVector2& v);
	SVector2& operator-=(const SVector2& v);
	SVector2& operator*=(float f);
	SVector2& operator/=(float f);

	SVector2& operator=(const SVector2& v);
	bool operator==(const SVector2& v) const;
	bool operator!=(const SVector2& v) const;
};

//----------------------------------------------------------------------------------------------------

struct SMatrix33
{
	float _11, _12, _13;
	float _21, _22, _23;
	float _31, _32, _33;

	// Constructors
	SMatrix33(void);
	SMatrix33(const SMatrix33& v);

	// Helper functions
	void Identity(void);
	void SetTranslate(float x, float y);
	void SetRotate(float rad);
	void SetRotate(const SVector2& dir, const SVector2& side);
	void SetScale(float x, float y);
	float Determinant(void);
	void Multiply(float scale);
	void Multiply(const SMatrix33& mat);
	void Inverse(void);
	SVector2 TransformCoord(const SVector2& v);
	SVector2 TransformNormal(const SVector2& v);
};

//----------------------------------------------------------------------------------------------------

struct SLineSegment
{
	SVector2 from;
	SVector2 to;

	// Constructors
	SLineSegment(void);
	SLineSegment(float x0, float y0, float x1, float y1);
	SLineSegment(const SVector2& v0, const SVector2& v1);
};

//----------------------------------------------------------------------------------------------------

struct SCircle
{
	SVector2 center;
	float radius;

	// Constructors
	SCircle(void);
	SCircle(float _x, float _y, float r);
	SCircle(const SVector2& c, float r);
};

//----------------------------------------------------------------------------------------------------

struct SRect
{
	SVector2 min;
	SVector2 max;

	// Constructors
	SRect(void);
	SRect(float l, float t, float r, float b);
	SRect(const SVector2& _min, const SVector2& _max);

	// Helper functions
	void Expand(const SVector2& point);

	bool IsValid(void) const					{ return (min.x < max.x) && (min.y < max.y); }

	SLineSegment GetLeftSegment(void) const		{ return SLineSegment(min.x, min.y, min.x, max.y); }
	SLineSegment GetRightSegment(void) const	{ return SLineSegment(max.x, min.y, max.x, max.y); }
	SLineSegment GetTopSegment(void) const		{ return SLineSegment(min.x, min.y, max.x, min.y); }
	SLineSegment GetBottomSegment(void) const	{ return SLineSegment(min.x, max.y, max.x, max.y); }

	float GetWidth(void) const					{ return max.x - min.x; }
	float GetHeight(void) const					{ return max.y - min.y; }

	// Overloaded operators
	SRect operator+(const SVector2& v) const;
	SRect operator+(const SRect& r) const;

	SRect& operator+=(const SVector2& v);
	SRect& operator+=(const SRect& r);
};

//====================================================================================================
// Helper Functions
//====================================================================================================

// Common math functions
float Min(float a, float b);
float Max(float a, float b);
float Clamp(float f, float min, float max);

// Functions to convert between degrees and radians
float DegToRad(float degree);
float RadToDeg(float radian);

// Function to return the unit vector
SVector2 Normalize(const SVector2& v);

// Function to return the dot product
float Dot(const SVector2& v0, const SVector2& v1);

// Functions to return the length of a vector
float Length(const SVector2& v);
float LengthSquared(const SVector2& v);

// Functions to calculate the distance between two points
float Distance(const SVector2& v0, const SVector2& v1);
float DistanceSquared(const SVector2& v0, const SVector2& v1);

// Function to check if a point is inside a rectangle
bool PointInRect(const SVector2& point, const SRect& rect);
bool PointInCircle(const SVector2& point, const SCircle& circle);

// Functions for intersect tests between different shapes
bool Intersect(const SLineSegment& a, const SLineSegment& b);
bool Intersect(const SCircle& c0, const SCircle& c1);
bool Intersect(const SRect& r0, const SRect& r1);
bool Intersect(const SLineSegment& l, const SRect& r1);

bool Intersect(const SLineSegment& l, const SCircle& c);
bool Intersect(const SCircle& c, const SLineSegment& l);

bool Intersect(const SCircle& c, const SRect& r);
bool Intersect(const SRect& r, const SCircle& c);

} // namespace SGE

#endif // #ifndef INCLUDED_SGE_MATH_H