#ifndef _SVECTOR2_H
#define _SVECTOR2_H

//====================================================================================================
// Constants
//====================================================================================================

static const float kPI		= 3.1415926535f;
static const float kEpsilon	= 0.000001f;


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
	SVector2 operator+(const SVector2& v) const;
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

#endif