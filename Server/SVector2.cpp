#include "SVector2.h"
#include <math.h>

float Dot(const SVector2& v0, const SVector2& v1)
{
	return (v0.x * v1.x) + (v0.y * v1.y);
}

SVector2::SVector2(void)
	: x(0.0f)
	, y(0.0f)
{}

//----------------------------------------------------------------------------------------------------

SVector2::SVector2(float _x, float _y)
	: x(_x)
	, y(_y)
{}

//----------------------------------------------------------------------------------------------------

SVector2::SVector2(const SVector2& v)
	: x(v.x)
	, y(v.y)
{}

//----------------------------------------------------------------------------------------------------

void SVector2::Invert(void)
{
	x = -x;
	y = -y;
}

//----------------------------------------------------------------------------------------------------

void SVector2::PerpendicularLH(void)
{
	float temp = x;
	x = -y;
	y = temp;
}

//----------------------------------------------------------------------------------------------------

void SVector2::PerpendicularRH(void)
{
	float temp = x;
	x = y;
	y = -temp;
}

//----------------------------------------------------------------------------------------------------

float SVector2::Length(void) const
{
	return sqrt((x * x) + (y * y));
}

//----------------------------------------------------------------------------------------------------

float SVector2::LengthSquared(void) const
{
	return ((x * x) + (y * y));
}

//----------------------------------------------------------------------------------------------------

void SVector2::Normalize(void)
{
	float mag = Length();
	if (mag != 0.0f)
	{
		x /= mag;
		y /= mag;
	}
}

//----------------------------------------------------------------------------------------------------

void SVector2::Truncate(float max)
{
	const float len = Length();
	if (len > kEpsilon)
	{
		const float ratio = max / len;
		if (ratio < 1.0f)
		{
			x *= ratio;
			y *= ratio;
		}
	}
}

//----------------------------------------------------------------------------------------------------

void SVector2::Reflect(const SVector2& normal)
{
	const SVector2 temp = normal * Dot(*this, normal);
	x = x - 2.0f * temp.x;
	y = y - 2.0f * temp.y;
}

//----------------------------------------------------------------------------------------------------

void SVector2::Rotate(float rad)
{
	const float kCosAngle = cos(rad);
	const float kSinAngle = sin(rad);

	SVector2 temp(x, y);
	x = temp.x * kCosAngle - temp.y * kSinAngle;
	y = temp.y * kCosAngle + temp.x * kSinAngle;
}

//----------------------------------------------------------------------------------------------------

SVector2 SVector2::operator+(const SVector2& v) const
{
	return SVector2(x + v.x, y + v.y);
}

//----------------------------------------------------------------------------------------------------

SVector2 SVector2::operator-(const SVector2& v) const
{
	return SVector2(x - v.x, y - v.y);
}

//----------------------------------------------------------------------------------------------------

SVector2 SVector2::operator*(float f) const
{
	return SVector2(x * f, y * f);
}

//----------------------------------------------------------------------------------------------------

SVector2 SVector2::operator/(float f) const
{
	const float inv = 1.0f / f;
	return SVector2(x * inv, y * inv);
}

//----------------------------------------------------------------------------------------------------

SVector2& SVector2::operator+=(const SVector2& v)
{
	x += v.x;
	y += v.y;
	return (*this);
}

//----------------------------------------------------------------------------------------------------

SVector2& SVector2::operator-=(const SVector2& v)
{
	x -= v.x;
	y -= v.y;
	return (*this);
}

//----------------------------------------------------------------------------------------------------

SVector2& SVector2::operator*=(float f)
{
	x *= f;
	y *= f;
	return (*this);
}

//----------------------------------------------------------------------------------------------------

SVector2& SVector2::operator/=(float f)
{
	const float inv = 1.0f / f;
	x *= inv;
	y *= inv;
	return (*this);
}

//----------------------------------------------------------------------------------------------------

SVector2& SVector2::operator=(const SVector2& v)
{
	x = v.x;
	y = v.y;
	return (*this);
}

//----------------------------------------------------------------------------------------------------

bool SVector2::operator==(const SVector2& v) const
{
	return (fabs(x - v.x) < kEpsilon && fabs(y - v.y) < kEpsilon);
}

//----------------------------------------------------------------------------------------------------

bool SVector2::operator!=(const SVector2& v) const
{
	return (x > v.x || x < v.x || y > v.y || y < v.y);
}

//----------------------------------------------------------------------------------------------------