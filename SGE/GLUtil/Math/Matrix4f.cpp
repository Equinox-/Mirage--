/*
 * Matrix4f.cpp
 *
 *  Created on: Aug 11, 2013
 *      Author: lwestin
 */

#include "Matrix4f.h"
#include <string.h>
#include <math.h>

Matrix4f::Matrix4f() {
	int r, c;
	for (r = 0; r < 4; r++) {
		for (c = 0; c < 4; c++) {
			set(r, c, (r == c ? 1.0 : 0.0));
		}
	}
}

Matrix4f::~Matrix4f() {
}

float Matrix4f::get(int row, int col) {
	return matrix[MATIDX(row,col)];
}

void Matrix4f::set(int row, int col, float f) {
	matrix[MATIDX(row,col)] = f;
}

Matrix4f Matrix4f::copy() {
	Matrix4f copy;
	memcpy(copy.matrix, matrix, 16 * sizeof(float));
	return copy;
}

Matrix4f Matrix4f::operator *=(Matrix4f a) {
	float data[16];
	int r, c, i;
	for (r = 0; r < 4; r++) {
		for (c = 0; c < 4; c++) {
			data[MATIDX(r,c)] = 0;
			for (i = 0; i < 4; i++) {
				data[MATIDX(r,c)] += get(r, i) * a.get(i, c);
			}
		}
	}
	memcpy(matrix, data, 16 * sizeof(float));
	return *this;
}

Matrix4f Matrix4f::operator *(Matrix4f a) {
	Matrix4f result;
	int r, c, i;
	for (r = 0; r < 4; r++) {
		for (c = 0; c < 4; c++) {
			result.matrix[MATIDX(r,c)] = 0;
			for (i = 0; i < 4; i++) {
				result.matrix[MATIDX(r,c)] += get(r, i) * a.get(i, c);
			}
		}
	}
	return result;
}

Matrix4f Matrix4f::operator *=(float f) {
	int r, c;
	for (r = 0; r < 4; r++) {
		for (c = 0; c < 4; c++) {
			matrix[MATIDX(r,c)] *= f;
		}
	}
	return *this;
}

Matrix4f Matrix4f::operator *(float f) {
	return copy() *= f;
}

Matrix4f Matrix4f::operator +=(Matrix4f f) {
	int r, c;
	for (r = 0; r < 4; r++) {
		for (c = 0; c < 4; c++) {
			matrix[MATIDX(r,c)] += f.get(r, c);
		}
	}
	return *this;
}

Matrix4f Matrix4f::operator -=(Matrix4f f) {
	int r, c;
	for (r = 0; r < 4; r++) {
		for (c = 0; c < 4; c++) {
			matrix[MATIDX(r,c)] -= f.get(r, c);
		}
	}
	return *this;
}

Matrix4f Matrix4f::operator -(Matrix4f f) {
	return copy() -= f;
}

Matrix4f Matrix4f::operator +(Matrix4f f) {
	return copy() += f;
}

Matrix4f Matrix4f::setRotation(float x, float y, float z, float radians) {
	float c = (float) cos(radians), s = (float) sin(radians);
	set(0, 0, c + (x * x * (1 - c)));
	set(0, 1, x * y * (1 - c) - z * s);
	set(0, 2, x * z * (1 - c) + y * s);

	set(1, 0, y * x * (1 - c) + z * s);
	set(1, 1, c + (y * y * (1 - c)));
	set(1, 2, y * z * (1 - c) - x * s);

	set(2, 0, z * x * (1 - c) - y * s);
	set(2, 1, z * y * (1 - c) + x * s);
	set(2, 2, c + (z * z * (1 - c)));
	return *this;
}

Vector3f Matrix4f::multiply(Vector3f v) {
	float resX = get(0, 3) + (v.x * get(0, 0)) + (v.y * get(0, 1))
			+ (v.z * get(0, 2));
	float resY = get(1, 3) + (v.x * get(1, 0)) + (v.y * get(1, 1))
			+ (v.z * get(1, 2));
	float resZ = get(2, 3) + (v.x * get(2, 0)) + (v.y * get(2, 1))
			+ (v.z * get(2, 2));
	return Vector3f((float) resX, (float) resY, (float) resZ);
}

Matrix4f Matrix4f::setTranslation(float x, float y, float z) {
	set(0, 3, x);
	set(1, 3, y);
	set(2, 3, z);
	set(3, 3, 1.0);
	return *this;
}

float Matrix4f::determinant() {
	return matrix[MATIDX(0, 0)]
			* (matrix[MATIDX(1, 1)]
					* (matrix[MATIDX(2, 2)] * matrix[MATIDX(3, 3)]
							- matrix[MATIDX(3, 2)] * matrix[MATIDX(2, 3)])
					- matrix[MATIDX(
							1, 2)]
							* (matrix[MATIDX(2, 1)] * matrix[MATIDX(3, 3)]
									- matrix[MATIDX(3, 1)]
											* matrix[MATIDX(2, 3)])
					+ matrix[MATIDX(1, 3)]
							* (matrix[MATIDX(
									2, 1)] * matrix[MATIDX(3, 2)]
									- matrix[MATIDX(3, 1)]
											* matrix[MATIDX(2, 2)]))
			- matrix[MATIDX(0, 1)]
					* (matrix[MATIDX(1, 0)]
							* (matrix[MATIDX(
									2, 2)] * matrix[MATIDX(3, 3)]
									- matrix[MATIDX(3, 2)]
											* matrix[MATIDX(2, 3)])
							- matrix[MATIDX(1, 2)]
									* (matrix[MATIDX(2, 0)] * matrix[MATIDX(
											3, 3)]
											- matrix[MATIDX(3, 0)]
													* matrix[MATIDX(2, 3)])
							+ matrix[MATIDX(1, 3)]
									* (matrix[MATIDX(2, 0)]
											* matrix[MATIDX(3, 2)]
											- matrix[MATIDX(
													3, 0)]
													* matrix[MATIDX(2, 2)]))
			+ matrix[MATIDX(0, 2)]
					* (matrix[MATIDX(1, 0)]
							* (matrix[MATIDX(2, 1)] * matrix[MATIDX(3, 3)]
									- matrix[MATIDX(
											3, 1)] * matrix[MATIDX(2, 3)])
							- matrix[MATIDX(1, 1)]
									* (matrix[MATIDX(2, 0)]
											* matrix[MATIDX(3, 3)]
											- matrix[MATIDX(3, 0)]
													* matrix[MATIDX(
															2, 3)])
							+ matrix[MATIDX(1, 3)]
									* (matrix[MATIDX(2, 0)]
											* matrix[MATIDX(3, 1)]
											- matrix[MATIDX(3, 0)]
													* matrix[MATIDX(2, 1)]))
			- matrix[MATIDX(
					0, 3)]
					* (matrix[MATIDX(1, 0)]
							* (matrix[MATIDX(2, 1)] * matrix[MATIDX(3, 2)]
									- matrix[MATIDX(3, 1)]
											* matrix[MATIDX(2, 2)])
							- matrix[MATIDX(
									1, 1)]
									* (matrix[MATIDX(2, 0)]
											* matrix[MATIDX(3, 2)]
											- matrix[MATIDX(3, 0)]
													* matrix[MATIDX(2, 2)])
							+ matrix[MATIDX(1, 2)]
									* (matrix[MATIDX(
											2, 0)] * matrix[MATIDX(3, 1)]
											- matrix[MATIDX(3, 0)]
													* matrix[MATIDX(2, 1)]));
}

Matrix4f Matrix4f::adjugate() {
	Matrix4f adj = Matrix4f();
	adj.matrix[MATIDX(0, 0)] = (matrix[MATIDX(1, 1)] * matrix[MATIDX(2, 2)]
			* matrix[MATIDX(3, 3)])
			+ (matrix[MATIDX(1, 2)] * matrix[MATIDX(2, 3)] * matrix[MATIDX(
					3, 1)])
			+ (matrix[MATIDX(1, 3)] * matrix[MATIDX(2, 1)]
					* matrix[MATIDX(3, 2)])
			- (matrix[MATIDX(1, 1)] * matrix[MATIDX(2, 3)] * matrix[MATIDX(
					3, 2)])
			- (matrix[MATIDX(1, 2)] * matrix[MATIDX(2, 1)]
					* matrix[MATIDX(3, 3)])
			- (matrix[MATIDX(1, 3)] * matrix[MATIDX(2, 2)] * matrix[MATIDX(
					3, 1)]);
	adj.matrix[MATIDX(0, 1)] = (matrix[MATIDX(0, 1)] * matrix[MATIDX(2, 3)]
			* matrix[MATIDX(3, 2)]) + (matrix[MATIDX(0, 2)] * matrix[MATIDX(
			2, 1)] * matrix[MATIDX(3, 3)])
			+ (matrix[MATIDX(0, 3)] * matrix[MATIDX(2, 2)]
					* matrix[MATIDX(3, 1)])
			- (matrix[MATIDX(0, 1)] * matrix[MATIDX(
					2, 2)] * matrix[MATIDX(3, 3)])
			- (matrix[MATIDX(0, 2)] * matrix[MATIDX(2, 3)]
					* matrix[MATIDX(3, 1)])
			- (matrix[MATIDX(0, 3)] * matrix[MATIDX(
					2, 1)] * matrix[MATIDX(3, 2)]);
	adj.matrix[MATIDX(0, 2)] = (matrix[MATIDX(0, 1)] * matrix[MATIDX(1, 2)]
			* matrix[MATIDX(3, 3)]) + (matrix[MATIDX(
			0, 2)] * matrix[MATIDX(1, 3)] * matrix[MATIDX(3, 1)])
			+ (matrix[MATIDX(0, 3)] * matrix[MATIDX(1, 1)]
					* matrix[MATIDX(3, 2)]) - (matrix[MATIDX(
			0, 1)] * matrix[MATIDX(1, 3)] * matrix[MATIDX(3, 2)])
			- (matrix[MATIDX(0, 2)] * matrix[MATIDX(1, 1)]
					* matrix[MATIDX(3, 3)]) - (matrix[MATIDX(
					0, 3)] * matrix[MATIDX(1, 2)] * matrix[MATIDX(3, 1)]);
	adj.matrix[MATIDX(0, 3)] = (matrix[MATIDX(0, 1)] * matrix[MATIDX(1, 3)]
			* matrix[MATIDX(
					2, 2)])
			+ (matrix[MATIDX(0, 2)] * matrix[MATIDX(1, 1)]
					* matrix[MATIDX(2, 3)])
			+ (matrix[MATIDX(0, 3)] * matrix[MATIDX(1, 2)] * matrix[MATIDX(
					2, 1)])
			- (matrix[MATIDX(0, 1)] * matrix[MATIDX(1, 2)]
					* matrix[MATIDX(2, 3)])
			- (matrix[MATIDX(0, 2)] * matrix[MATIDX(1, 3)] * matrix[MATIDX(
					2, 1)])
			- (matrix[MATIDX(0, 3)] * matrix[MATIDX(1, 1)]
					* matrix[MATIDX(2, 2)]);

	adj.matrix[MATIDX(1, 0)] = (matrix[MATIDX(1, 0)] * matrix[MATIDX(
			2, 3)] * matrix[MATIDX(3, 2)])
			+ (matrix[MATIDX(1, 2)] * matrix[MATIDX(2, 0)]
					* matrix[MATIDX(3, 3)])
			+ (matrix[MATIDX(1, 3)] * matrix[MATIDX(
					2, 2)] * matrix[MATIDX(3, 0)])
			- (matrix[MATIDX(1, 0)] * matrix[MATIDX(2, 2)]
					* matrix[MATIDX(3, 3)])
			- (matrix[MATIDX(1, 2)] * matrix[MATIDX(
					2, 3)] * matrix[MATIDX(3, 0)])
			- (matrix[MATIDX(1, 3)] * matrix[MATIDX(2, 0)]
					* matrix[MATIDX(3, 2)]);
	adj.matrix[MATIDX(1, 1)] = (matrix[MATIDX(
			0, 0)] * matrix[MATIDX(2, 2)] * matrix[MATIDX(3, 3)])
			+ (matrix[MATIDX(0, 2)] * matrix[MATIDX(2, 3)]
					* matrix[MATIDX(3, 0)]) + (matrix[MATIDX(
					0, 3)] * matrix[MATIDX(2, 0)] * matrix[MATIDX(3, 2)])
			- (matrix[MATIDX(0, 0)] * matrix[MATIDX(2, 3)]
					* matrix[MATIDX(3, 2)]) - (matrix[MATIDX(
					0, 2)] * matrix[MATIDX(2, 0)] * matrix[MATIDX(3, 3)])
			- (matrix[MATIDX(0, 3)] * matrix[MATIDX(2, 2)]
					* matrix[MATIDX(3, 0)]);
	adj.matrix[MATIDX(
			1, 2)] = (matrix[MATIDX(0, 0)] * matrix[MATIDX(1, 3)]
			* matrix[MATIDX(3, 2)])
			+ (matrix[MATIDX(0, 2)] * matrix[MATIDX(1, 0)] * matrix[MATIDX(
					3, 3)])
			+ (matrix[MATIDX(0, 3)] * matrix[MATIDX(1, 2)]
					* matrix[MATIDX(3, 0)])
			- (matrix[MATIDX(0, 0)] * matrix[MATIDX(1, 2)] * matrix[MATIDX(
					3, 3)])
			- (matrix[MATIDX(0, 2)] * matrix[MATIDX(1, 0)]
					* matrix[MATIDX(3, 0)])
			- (matrix[MATIDX(0, 3)] * matrix[MATIDX(1, 0)] * matrix[MATIDX(
					3, 2)]);
	adj.matrix[MATIDX(1, 3)] = (matrix[MATIDX(0, 0)] * matrix[MATIDX(1, 2)]
			* matrix[MATIDX(2, 3)]) + (matrix[MATIDX(0, 2)] * matrix[MATIDX(
			1, 3)] * matrix[MATIDX(2, 0)])
			+ (matrix[MATIDX(0, 3)] * matrix[MATIDX(1, 0)]
					* matrix[MATIDX(2, 2)])
			- (matrix[MATIDX(0, 0)] * matrix[MATIDX(
					1, 3)] * matrix[MATIDX(2, 2)])
			- (matrix[MATIDX(0, 2)] * matrix[MATIDX(1, 0)]
					* matrix[MATIDX(2, 3)])
			- (matrix[MATIDX(0, 3)] * matrix[MATIDX(
					1, 2)] * matrix[MATIDX(2, 0)]);

	adj.matrix[MATIDX(2, 0)] = (matrix[MATIDX(1, 0)] * matrix[MATIDX(2, 1)]
			* matrix[MATIDX(3, 3)]) + (matrix[MATIDX(
			1, 1)] * matrix[MATIDX(2, 3)] * matrix[MATIDX(3, 0)])
			+ (matrix[MATIDX(1, 3)] * matrix[MATIDX(2, 0)]
					* matrix[MATIDX(3, 1)]) - (matrix[MATIDX(
			1, 0)] * matrix[MATIDX(2, 3)] * matrix[MATIDX(3, 1)])
			- (matrix[MATIDX(1, 1)] * matrix[MATIDX(2, 0)]
					* matrix[MATIDX(3, 3)]) - (matrix[MATIDX(
					1, 3)] * matrix[MATIDX(2, 1)] * matrix[MATIDX(3, 0)]);
	adj.matrix[MATIDX(2, 1)] = (matrix[MATIDX(0, 0)] * matrix[MATIDX(2, 3)]
			* matrix[MATIDX(
					3, 1)])
			+ (matrix[MATIDX(0, 1)] * matrix[MATIDX(2, 0)]
					* matrix[MATIDX(3, 3)])
			+ (matrix[MATIDX(0, 3)] * matrix[MATIDX(2, 1)] * matrix[MATIDX(
					3, 0)])
			- (matrix[MATIDX(0, 0)] * matrix[MATIDX(2, 1)]
					* matrix[MATIDX(3, 3)])
			- (matrix[MATIDX(0, 1)] * matrix[MATIDX(2, 3)] * matrix[MATIDX(
					3, 0)])
			- (matrix[MATIDX(0, 3)] * matrix[MATIDX(2, 0)]
					* matrix[MATIDX(3, 1)]);
	adj.matrix[MATIDX(2, 2)] = (matrix[MATIDX(0, 0)] * matrix[MATIDX(
			1, 1)] * matrix[MATIDX(3, 3)])
			+ (matrix[MATIDX(0, 1)] * matrix[MATIDX(1, 3)]
					* matrix[MATIDX(3, 0)])
			+ (matrix[MATIDX(0, 3)] * matrix[MATIDX(
					1, 0)] * matrix[MATIDX(3, 1)])
			- (matrix[MATIDX(0, 0)] * matrix[MATIDX(1, 3)]
					* matrix[MATIDX(3, 1)])
			- (matrix[MATIDX(0, 1)] * matrix[MATIDX(
					1, 0)] * matrix[MATIDX(3, 3)])
			- (matrix[MATIDX(0, 3)] * matrix[MATIDX(1, 1)]
					* matrix[MATIDX(3, 0)]);
	adj.matrix[MATIDX(2, 3)] = (matrix[MATIDX(
			0, 0)] * matrix[MATIDX(1, 3)] * matrix[MATIDX(2, 1)])
			+ (matrix[MATIDX(0, 1)] * matrix[MATIDX(1, 0)]
					* matrix[MATIDX(2, 3)]) + (matrix[MATIDX(
					0, 3)] * matrix[MATIDX(1, 1)] * matrix[MATIDX(2, 0)])
			- (matrix[MATIDX(0, 0)] * matrix[MATIDX(1, 1)]
					* matrix[MATIDX(2, 3)]) - (matrix[MATIDX(
					0, 1)] * matrix[MATIDX(1, 3)] * matrix[MATIDX(2, 0)])
			- (matrix[MATIDX(0, 3)] * matrix[MATIDX(1, 0)]
					* matrix[MATIDX(2, 1)]);

	adj.matrix[MATIDX(
			3, 0)] = (matrix[MATIDX(1, 0)] * matrix[MATIDX(2, 2)]
			* matrix[MATIDX(3, 1)])
			+ (matrix[MATIDX(1, 1)] * matrix[MATIDX(2, 0)] * matrix[MATIDX(
					3, 2)])
			+ (matrix[MATIDX(1, 2)] * matrix[MATIDX(2, 1)]
					* matrix[MATIDX(3, 0)])
			- (matrix[MATIDX(1, 0)] * matrix[MATIDX(2, 1)] * matrix[MATIDX(
					3, 2)])
			- (matrix[MATIDX(1, 1)] * matrix[MATIDX(2, 2)]
					* matrix[MATIDX(3, 0)])
			- (matrix[MATIDX(1, 2)] * matrix[MATIDX(2, 0)] * matrix[MATIDX(
					3, 1)]);
	adj.matrix[MATIDX(3, 1)] = (matrix[MATIDX(0, 0)] * matrix[MATIDX(2, 1)]
			* matrix[MATIDX(3, 1)]) + (matrix[MATIDX(0, 1)] * matrix[MATIDX(
			2, 2)] * matrix[MATIDX(3, 0)])
			+ (matrix[MATIDX(0, 2)] * matrix[MATIDX(2, 0)]
					* matrix[MATIDX(3, 1)])
			- (matrix[MATIDX(0, 0)] * matrix[MATIDX(
					2, 2)] * matrix[MATIDX(3, 1)])
			- (matrix[MATIDX(0, 1)] * matrix[MATIDX(2, 0)]
					* matrix[MATIDX(3, 2)])
			- (matrix[MATIDX(0, 2)] * matrix[MATIDX(
					2, 1)] * matrix[MATIDX(3, 0)]);
	adj.matrix[MATIDX(3, 2)] = (matrix[MATIDX(0, 0)] * matrix[MATIDX(1, 2)]
			* matrix[MATIDX(3, 1)]) + (matrix[MATIDX(
			0, 1)] * matrix[MATIDX(1, 0)] * matrix[MATIDX(3, 2)])
			+ (matrix[MATIDX(0, 2)] * matrix[MATIDX(1, 1)]
					* matrix[MATIDX(3, 0)]) - (matrix[MATIDX(
			0, 0)] * matrix[MATIDX(1, 1)] * matrix[MATIDX(3, 2)])
			- (matrix[MATIDX(0, 1)] * matrix[MATIDX(1, 2)]
					* matrix[MATIDX(3, 0)]) - (matrix[MATIDX(
					0, 2)] * matrix[MATIDX(1, 0)] * matrix[MATIDX(3, 1)]);
	adj.matrix[MATIDX(3, 3)] = (matrix[MATIDX(0, 0)] * matrix[MATIDX(1, 1)]
			* matrix[MATIDX(
					2, 2)])
			+ (matrix[MATIDX(0, 1)] * matrix[MATIDX(1, 2)]
					* matrix[MATIDX(2, 0)])
			+ (matrix[MATIDX(0, 2)] * matrix[MATIDX(1, 0)] * matrix[MATIDX(
					2, 1)])
			- (matrix[MATIDX(0, 0)] * matrix[MATIDX(1, 2)]
					* matrix[MATIDX(2, 1)])
			- (matrix[MATIDX(0, 1)] * matrix[MATIDX(1, 0)] * matrix[MATIDX(
					2, 2)])
			- (matrix[MATIDX(0, 2)] * matrix[MATIDX(1, 1)]
					* matrix[MATIDX(2, 0)]);
	return adj;
}

Matrix4f Matrix4f::inverse() {
	Matrix4f m = adjugate();
	m *= (1.0 / determinant());
	return m;
}
