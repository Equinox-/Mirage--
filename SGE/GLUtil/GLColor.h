/*
 * GLColor.h
 *
 *  Created on: Aug 8, 2013
 *      Author: westin
 */

#ifndef GLCOLOR_H_
#define GLCOLOR_H_

class GLColor {
private:
public:
	float r, g, b, a;
	GLColor();
	GLColor(float r, float g, float b, float a = 1.0);
	GLColor(int value);
	virtual ~GLColor();
	void setValue(int value);
	unsigned int getValue();

	void bind();

	GLColor operator |(int i);
};

#endif /* GLCOLOR_H_ */
