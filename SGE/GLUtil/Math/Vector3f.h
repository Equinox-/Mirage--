/*
 * Vector3f.h
 *
 *  Created on: Aug 8, 2013
 *      Author: lwestin
 */

#ifndef VECTOR3F_H_
#define VECTOR3F_H_

class Vector3f {
public:
	float x, y, z;
	Vector3f();
	Vector3f(float, float, float);
	virtual ~Vector3f();

	Vector3f normalize();
	float magnitude();
	Vector3f clone();

	inline Vector3f operator +(Vector3f);
	inline Vector3f operator +=(Vector3f);

	inline Vector3f operator -(Vector3f);
	inline Vector3f operator -=(Vector3f);

	inline Vector3f operator *(float);
	inline Vector3f operator *=(float);

	inline bool operator ==(Vector3f);
	inline bool operator !=(Vector3f);

	static float dotProduct(Vector3f, Vector3f);
	static Vector3f crossProduct(Vector3f, Vector3f);
};

#endif /* VECTOR3F_H_ */
