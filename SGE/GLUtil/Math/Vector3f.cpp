/*
 * Vector3f.cpp
 *
 *  Created on: Aug 8, 2013
 *      Author: lwestin
 */

#include "Vector3f.h"
#include <math.h>

Vector3f::Vector3f() :
		x(0), y(0), z(0) {

}

Vector3f::Vector3f(float a, float b, float c) :
		x(a), y(b), z(c) {
}

Vector3f::~Vector3f() {
}

Vector3f Vector3f::operator -(Vector3f a) {
	return this->clone() -= a;
}

Vector3f Vector3f::operator -=(Vector3f a) {
	this->x -= a.x;
	this->y -= a.y;
	this->z -= a.z;
	return *(this);
}

Vector3f Vector3f::operator +(Vector3f a) {
	return this->clone() += a;
}

Vector3f Vector3f::operator +=(Vector3f a) {
	this->x += a.x;
	this->y += a.y;
	this->z += a.z;
	return *(this);
}

Vector3f Vector3f::operator *(float s) {
	return this->clone() *= s;
}

Vector3f Vector3f::operator *=(float s) {
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *(this);
}

bool Vector3f::operator ==(Vector3f a) {
	return a.x == this->x && a.y == this->y && a.z == this->z;
}

bool Vector3f::operator !=(Vector3f a) {
	return !(*(this) == a);
}

Vector3f Vector3f::clone() {
	return Vector3f(this->x, this->y, this->z);
}

Vector3f Vector3f::normalize() {
	float mag = magnitude();
	if (mag != 0) {
		this->x /= mag;
		this->y /= mag;
		this->z /= mag;
	}
	return *(this);
}

float Vector3f::magnitude() {
	return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
}

float Vector3f::dotProduct(Vector3f u, Vector3f v) {
	return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

Vector3f Vector3f::crossProduct(Vector3f u, Vector3f v) {
	return Vector3f((u.y * v.z) - (u.z * v.y), (u.z * v.x) - (u.x * v.z),
			(u.x * v.y) - (u.y * v.x));
}
