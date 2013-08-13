/*
 * GLFont.h
 *
 *  Created on: Aug 8, 2013
 *      Author: lwestin
 */

#ifndef GLFONT_H_
#define GLFONT_H_

#include "GLTexture.h"
#include "Math/Rectangle.h"
#include "../Graphics/CTexture.h"
class GLFont {
private:
	int start, end;
	int charCountX, charCountY;
	GLTexture *fontTexture;
	CTexture *cTexture;
	float gridWidth, gridHeight;
	float gridPWidth, gridPHeight;

	float *charRight;
	float *charLeft;
	float scaleSize;
	void calculateCharSize(int c);
public:
	GLFont(int, int, int, char[], bool, float scale = 1.0);
	void render(const char*, Rectangle);
	virtual ~GLFont();
	void initialize();
	void dispose();
};

#endif /* GLFONT_H_ */
