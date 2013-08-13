//====================================================================================================
// Filename:	SGE_Math.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "SGE_Math.h"
//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

	//====================================================================================================
	// Structs
	//====================================================================================================

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

	SMatrix33::SMatrix33(void)
		: _11(1.0f), _12(0.0f), _13(0.0f)
		, _21(0.0f), _22(1.0f), _23(0.0f)
		, _31(0.0f), _32(0.0f), _33(1.0f)
	{}

	//----------------------------------------------------------------------------------------------------

	SMatrix33::SMatrix33(const SMatrix33& v)
		: _11(v._11), _12(v._12), _13(v._13)
		, _21(v._21), _22(v._22), _23(v._23)
		, _31(v._31), _32(v._32), _33(v._33)
	{}

	//----------------------------------------------------------------------------------------------------

	void SMatrix33::Identity(void)
	{
		_11 = 1.0f; _12 = 0.0f; _13 = 0.0f;
		_21 = 0.0f; _22 = 1.0f; _23 = 0.0f;
		_31 = 0.0f; _32 = 0.0f; _33 = 1.0f;
	}

	//----------------------------------------------------------------------------------------------------

	void SMatrix33::SetTranslate(float x, float y)
	{
		_11 = 1.0f;	_12 = 0.0f;	_13 = 0.0f;
		_21 = 0.0f;	_22 = 1.0f;	_23 = 0.0f;
		_31 = x;	_32 = y;	_33 = 1.0f;
	}

	//----------------------------------------------------------------------------------------------------

	void SMatrix33::SetRotate(float rad)
	{
		const float c = cos(rad);
		const float s = sin(rad);

		_11 = c;	_12 = s;	_13 = 0.0f;
		_21 = -s;	_22 = c;	_23 = 0.0f;
		_31 = 0.0f;	_32 = 0.0f;	_33 = 1.0f;
	}

	//----------------------------------------------------------------------------------------------------

	void SMatrix33::SetRotate(const SVector2& dir, const SVector2& side)
	{
		_11 = dir.x;	_12 = dir.y;	_13 = 0.0f;
		_21 = side.x;	_22 = side.y;	_23 = 0.0f;
		_31 = 0.0f;		_32 = 0.0f;		_33 = 1.0f;
	}

	//----------------------------------------------------------------------------------------------------

	void SMatrix33::SetScale(float x, float y)
	{
		_11 = x;	_12 = 0.0f;	_13 = 0.0f;
		_21 = 0.0f;	_22 = y;	_23 = 0.0f;
		_31 = 0.0f;	_32 = 0.0f;	_33 = 1.0f;
	}

	//----------------------------------------------------------------------------------------------------

	float SMatrix33::Determinant(void)
	{
		const float A = _11 * (_22 * _33 - _23 * _32);
		const float B = _12 * (_21 * _33 - _23 * _31);
		const float C = _13 * (_21 * _32 - _22 * _31);
		return (A - B + C);
	}

	//----------------------------------------------------------------------------------------------------

	void SMatrix33::Multiply(float scale)
	{
		_11 *= scale; _12 *= scale; _13 *= scale;
		_21 *= scale; _22 *= scale; _23 *= scale;
		_31 *= scale; _32 *= scale; _33 *= scale;
	}

	//----------------------------------------------------------------------------------------------------

	void SMatrix33::Multiply(const SMatrix33& mat)
	{
		SMatrix33 temp(*this);

		_11 = (temp._11 * mat._11) + (temp._12 * mat._21) + (temp._13 * mat._31);
		_12 = (temp._11 * mat._12) + (temp._12 * mat._22) + (temp._13 * mat._32);
		_13 = (temp._11 * mat._13) + (temp._12 * mat._23) + (temp._13 * mat._33);

		_21 = (temp._21 * mat._11) + (temp._22 * mat._21) + (temp._23 * mat._31);
		_22 = (temp._21 * mat._12) + (temp._22 * mat._22) + (temp._23 * mat._32);
		_23 = (temp._21 * mat._13) + (temp._22 * mat._23) + (temp._23 * mat._33);

		_31 = (temp._31 * mat._11) + (temp._32 * mat._21) + (temp._33 * mat._31);
		_32 = (temp._31 * mat._12) + (temp._32 * mat._22) + (temp._33 * mat._32);
		_33 = (temp._31 * mat._13) + (temp._32 * mat._23) + (temp._33 * mat._33);
	}

	//----------------------------------------------------------------------------------------------------

	void SMatrix33::Inverse(void)
	{
		SMatrix33 temp(*this);
		const float invDet = 1.0f / Determinant();

		_11 = (temp._22 * temp._33 - temp._23 * temp._32) * invDet;
		_12 = (temp._13 * temp._32 - temp._12 * temp._33) * invDet;
		_13 = (temp._12 * temp._23 - temp._13 * temp._22) * invDet;

		_21 = (temp._23 * temp._31 - temp._21 * temp._33) * invDet;
		_22 = (temp._11 * temp._33 - temp._13 * temp._31) * invDet;
		_23 = (temp._13 * temp._21 - temp._11 * temp._23) * invDet;

		_31 = (temp._21 * temp._32 - temp._22 * temp._31) * invDet;
		_32 = (temp._12 * temp._31 - temp._11 * temp._32) * invDet;
		_33 = (temp._11 * temp._22 - temp._12 * temp._21) * invDet;
	}

	//----------------------------------------------------------------------------------------------------

	SVector2 SMatrix33::TransformCoord(const SVector2& v)
	{
		return SVector2((v.x * _11) + (v.y * _21) + (1.0f * _31),
			(v.x * _12) + (v.y * _22) + (1.0f * _32));
	}

	//----------------------------------------------------------------------------------------------------

	SVector2 SMatrix33::TransformNormal(const SVector2& v)
	{
		return SVector2((v.x * _11) + (v.y * _21), (v.x * _12) + (v.y * _22));
	}

	//----------------------------------------------------------------------------------------------------

	SLineSegment::SLineSegment(void)
		: from(0.0f, 0.0f)
		, to(0.0f, 0.0f)
	{}

	//----------------------------------------------------------------------------------------------------

	SLineSegment::SLineSegment(float x0, float y0, float x1, float y1)
		: from(x0, y0)
		, to(x1, y1)
	{}

	//----------------------------------------------------------------------------------------------------

	SLineSegment::SLineSegment(const SVector2& v0, const SVector2& v1)
		: from(v0)
		, to(v1)
	{}

	//----------------------------------------------------------------------------------------------------

	SCircle::SCircle(void)
		: center(0.0f, 0.0f)
		, radius(0.0f)
	{}

	//----------------------------------------------------------------------------------------------------

	SCircle::SCircle(float _x, float _y, float r)
		: center(_x, _y)
		, radius(r)
	{}

	//----------------------------------------------------------------------------------------------------

	SCircle::SCircle(const SVector2& c, float r)
		: center(c)
		, radius(r)
	{}

	//----------------------------------------------------------------------------------------------------

	SRect::SRect(void)
		: min(FLT_MAX, FLT_MAX)
		, max(-FLT_MAX, -FLT_MAX)
	{}

	//----------------------------------------------------------------------------------------------------

	SRect::SRect(float l, float t, float r, float b)
		: min(l, t)
		, max(r, b)
	{}

	//----------------------------------------------------------------------------------------------------

	SRect::SRect(const SVector2& _min, const SVector2& _max)
		: min(_min)
		, max(_max)
	{}

	//----------------------------------------------------------------------------------------------------

	void SRect::Expand(const SVector2& point)
	{
		min.x = Min(min.x, point.x);
		min.y = Min(min.y, point.y);
		max.x = Max(max.x, point.x);
		max.y = Max(max.y, point.y);
	}

	//----------------------------------------------------------------------------------------------------

	SRect SRect::operator+(const SVector2& v) const
	{
		return SRect(min + v, max + v);
	}

	//----------------------------------------------------------------------------------------------------

	SRect SRect::operator+(const SRect& r) const
	{
		return SRect
			(
			Min(min.x, r.min.x),
			Min(min.y, r.min.y),
			Max(max.x, r.max.x),
			Max(max.y, r.max.y)
			);
	}

	//----------------------------------------------------------------------------------------------------

	SRect& SRect::operator+=(const SVector2& v)
	{
		min += v;
		max += v;
		return (*this);
	}

	//----------------------------------------------------------------------------------------------------

	SRect& SRect::operator+=(const SRect& r)
	{
		min.x = Min(min.x, r.min.x);
		min.y = Min(min.y, r.min.y);
		max.x = Max(max.x, r.max.x);
		max.y = Max(max.y, r.max.y);
		return (*this);
	}

	//====================================================================================================
	// Helper Functions
	//====================================================================================================

	float Min(float a, float b)
	{
		return (a <= b) ? a : b;
	}

	//----------------------------------------------------------------------------------------------------

	float Max(float a, float b)
	{
		return (a > b) ? a : b;
	}

	//----------------------------------------------------------------------------------------------------

	float Clamp(float f, float min, float max)
	{
		float result = f;
		if (f < min)
		{
			result = min;
		}
		if (f > max)
		{
			result = max;
		}
		return result;
	}

	//----------------------------------------------------------------------------------------------------

	float DegToRad(float degree)
	{
		return degree * kPI / 180.0f;
	}

	//----------------------------------------------------------------------------------------------------

	float RadToDeg(float radian)
	{
		return radian * 180.0f / kPI;
	}

	//----------------------------------------------------------------------------------------------------

	SVector2 Normalize(const SVector2& v)
	{
		SVector2 norm(v);
		norm.Normalize();
		return norm;
	}

	//----------------------------------------------------------------------------------------------------

	float Dot(const SVector2& v0, const SVector2& v1)
	{
		return (v0.x * v1.x) + (v0.y * v1.y);
	}

	//----------------------------------------------------------------------------------------------------

	float Length(const SVector2& v)
	{
		return v.Length();
	}

	//----------------------------------------------------------------------------------------------------

	float LengthSquared(const SVector2& v)
	{
		return v.LengthSquared();
	}

	//----------------------------------------------------------------------------------------------------

	float Distance(const SVector2& v0, const SVector2& v1)
	{
		float fDiffX = v0.x - v1.x;
		float fDiffY = v0.y - v1.y;

		return sqrt(fDiffX * fDiffX + fDiffY * fDiffY);
	}

	//----------------------------------------------------------------------------------------------------

	float DistanceSquared(const SVector2& v0, const SVector2& v1)
	{
		float fDiffX = v0.x - v1.x;
		float fDiffY = v0.y - v1.y;

		return (fDiffX * fDiffX + fDiffY * fDiffY);
	}

	//----------------------------------------------------------------------------------------------------

	bool PointInRect(const SVector2& point, const SRect& rect)
	{
		if (point.x > rect.max.x || point.x < rect.min.x ||
			point.y > rect.max.y || point.y < rect.min.y)
		{
			return false;
		}
		return true;
	}

	//----------------------------------------------------------------------------------------------------

	bool PointInCircle(const SVector2& point, const SCircle& circle)
	{
		const SVector2 centerToPoint = point - circle.center;
		const float distSqr = Dot(centerToPoint, centerToPoint);
		const float radiusSqr = circle.radius * circle.radius;
		return distSqr < radiusSqr;
	}

	//----------------------------------------------------------------------------------------------------


bool Intersect(const SLineSegment& a, const SLineSegment& b)
{
    // http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

    float ua = ((b.to.x - b.from.x) * (a.from.y - b.from.y)) - ((b.to.y - b.from.y) * (a.from.x - b.from.x));
    float ub = ((a.to.x - a.from.x) * (a.from.y - b.from.y)) - ((a.to.y - a.from.y) * (a.from.x - b.from.x));
    float denom = ((b.to.y - b.from.y) * (a.to.x - a.from.x)) - ((b.to.x - b.from.x) * (a.to.y - a.from.y));

    // First check for special cases
    if (denom == 0.0f)
    {
        if (ua == 0.0f && ub == 0.0f)
        {
            // The line segments are the same
            return false;
        }
        else
        {
            // The line segments are parallel
            return false;
        }
    }

    ua /= denom;
    ub /= denom;

    if (ua < 0.0f || ua > 1.0f || ub < 0.0f || ub > 1.0f)
    {
        return false;
    }

    return true;
}
	//----------------------------------------------------------------------------------------------------

	bool Intersect(const SCircle& c0, const SCircle& c1)
	{
		const float fRadii = c0.radius + c1.radius;
		const float fRadiiSquared = fRadii * fRadii;
		const float fDistanceSquared = DistanceSquared(c0.center, c1.center);
		return (fDistanceSquared < fRadiiSquared);
	}

	//----------------------------------------------------------------------------------------------------

	bool Intersect(const SRect& r0, const SRect& r1)
	{
		if (!r0.IsValid() || !r1.IsValid())
			return false;
		else if (r0.min.x > r1.max.x)
			return false;
		else if (r0.min.y > r1.max.y)
			return false;
		else if (r0.max.x < r1.min.x)
			return false;
		else if (r0.max.y < r1.min.y)
			return false;
		return true;
	}

	bool Intersect(const SLineSegment& l, const SRect& r1)
	{
		return Intersect(l, r1.GetBottomSegment()) || Intersect(l, r1.GetTopSegment()) || Intersect(l, r1.GetLeftSegment()) || Intersect(l, r1.GetRightSegment());
	}
	//----------------------------------------------------------------------------------------------------

	bool Intersect(const SLineSegment& l, const SCircle& c)
	{
		return Intersect(c, l);
	}

	//----------------------------------------------------------------------------------------------------

	bool Intersect(const SCircle& c, const SLineSegment& l)
	{
		SVector2 startToCenter = c.center - l.from;
		SVector2 startToEnd = l.to - l.from;
		float len = startToEnd.Length();
		SVector2 dir = startToEnd / len;

		// Find the closest point to the line segment
		float projection = Dot(startToCenter, dir);
		SVector2 closestPoint;
		if (projection > len)
		{
			closestPoint = l.to;
		}
		else if (projection < 0.0f)
		{
			closestPoint = l.from;
		}
		else
		{
			closestPoint = l.from + (dir * projection);
		}

		// Check if the closest point is within the circle
		SVector2 closestToCenter = c.center - closestPoint;
		if (closestToCenter.LengthSquared() > c.radius * c.radius)
		{
			return false;
		}
		return true;
	}

	//----------------------------------------------------------------------------------------------------

	bool Intersect(const SCircle& c, const SRect& r)
	{
		return Intersect(r, c);
	}

	//----------------------------------------------------------------------------------------------------

	bool Intersect(const SRect& r, const SCircle& c)
	{
		if (!r.IsValid())
		{
			return false;
		}

		SVector2 closestPoint;
		closestPoint.x = Clamp(c.center.x, r.min.x, r.max.x);
		closestPoint.y = Clamp(c.center.y, r.min.y, r.max.y);

		const float distance = Distance(closestPoint, c.center);
		if (distance > c.radius)
		{
			return false;
		}
		return true;
	}

} // namespace SGE