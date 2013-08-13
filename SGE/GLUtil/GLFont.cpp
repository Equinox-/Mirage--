/*
 * GLFont.cpp
 *
 *  Created on: Aug 8, 2013
 *      Author: lwestin
 */

#include "GLFont.h"
#include "../Resource/CResourceManager.h"
#include "../Graphics/CTexture.h"
#include "GL/gl.h"

GLFont::GLFont(int offset, int charCountX, int charCountY, char texFile[],
		bool monospaced, float scale) :
		start(offset), charCountX(charCountX), charCountY(charCountY), scaleSize(
				scale) {
	//fontTexture->loadFromFile(texFile);
	cTexture = CResourceManager::Get()->Create<CTexture>(texFile);
	fontTexture = cTexture->GetTexture();
	end = offset + charCountX * charCountY;
	gridWidth = 1.0 / ((float) charCountX);
	gridHeight = 1.0 / ((float) charCountY);
	gridPWidth = ((float) fontTexture->getWidth()) / ((float) charCountX);
	gridPHeight = ((float) fontTexture->getHeight()) / ((float) charCountY);
	if (monospaced) {
		charRight = NULL;
		charLeft = NULL;
	} else {
		charRight = new float[end - start];
		charLeft = new float[end - start];
		for (register int i = start; i < end; i++) {
			calculateCharSize(i);
		}

		calculateCharSize(56);
	}
}

void GLFont::calculateCharSize(int i) {
	int cID = i - start;
	int cX = (cID % charCountX) * gridPWidth;
	int cY = ((cID / charCountX) * gridPHeight);
	charRight[cID] = 0;
	for (int x = cX + gridPWidth - 1; x >= cX && charRight[cID] == 0; x--) {
		for (int y = cY; y < cY + gridPHeight; y++) {
			GLColor c = fontTexture->getColorAt(x, y); // Icky object allocatation TODO
			if (c.a > 0.0) {
				if (x - cX == gridPHeight - 1) {
					return;
				}
				charRight[cID] = x - cX + 1;
				break;
			}
		}
	}
	charLeft[cID] = 0;
	for (int x = cX; x < cX + charRight[cID] && charRight[cID] != 0; x++) {
		for (int y = cY; y < cY + gridPHeight; y++) {
			GLColor c = fontTexture->getColorAt(x, y); // Icky object allocatation TODO
			if (c.a > 0.0) {
				charLeft[cID] = x - cX - 1;
				return;
			}
		}
	}
	// A totally empty square.  What fun.  Howabouts we eyeball it's size
	charLeft[cID] = gridPWidth / 4;
	charRight[cID] = gridPWidth / 4 * 3;
}

GLFont::~GLFont() {
	fontTexture->freeRawData();
	if (charLeft != NULL) {
		delete[] charLeft;
	}
	if (charRight != NULL) {
		delete[] charRight;
	}
}

void GLFont::initialize() {
	fontTexture->loadToVRAM();
}

void GLFont::dispose() {
	CResourceManager::Get()->Destroy(cTexture);
}

void GLFont::render(const char* text, Rectangle rect) {	// TODO rect is a place to wrap stuff.  Needs options
	register int i = 0;
	fontTexture->bind();
	glBegin(GL_QUADS);
	//glColor3f(1, 1, 1);
	float x = 0;

	/*float totWidth = 0.0;
	 while (text[i] != 0) {
	 if (text[i] != 0) {
	 int cID = (unsigned char) text[i] - start;
	 if (cID < 0 || cID >= end - start) {
	 cID = 0;
	 }
	 if (charRight != NULL && charRight[cID] > 0) {
	 totWidth += charRight[cID] - charLeft[cID];
	 } else {
	 totWidth += gridPWidth;
	 }
	 }
	 ++i;
	 }
	 i = 0;*/

	float yMult = scaleSize;		//(rect.bottom - rect.top) / gridPHeight;
	float xMult = scaleSize;		//(rect.right - rect.left) / gridPWidth;

	float charWidth = gridWidth;
	float charPWidth = gridPWidth;
	while (text[i] != 0) {
		if (text[i] == '\n') {
			rect.top += gridPHeight * yMult;
			x = 0;
		} else if (text[i] != 0) {
			int cID = (unsigned char) text[i] - start;
			if (cID < 0 || cID >= end - start) {
				cID = 0;
			}

			float cX = (cID % charCountX) * gridWidth;
			float cY = 1 - ((cID / charCountX) * gridHeight);
			if (charRight != NULL && charRight[cID] > 0) {
				charPWidth = charRight[cID] - charLeft[cID];
				charWidth = charPWidth / (float) fontTexture->getWidth();
				cX += (charLeft[cID] / (float) fontTexture->getWidth());
			} else {
				charWidth = gridWidth;
				charPWidth = gridPWidth;
			}

			glTexCoord2f(cX, cY);
			glVertex2f(x * xMult + rect.left, rect.top);

			glTexCoord2f(cX + charWidth, cY);
			glVertex2f((x + charPWidth) * xMult + rect.left, rect.top);

			glTexCoord2f(cX + charWidth, cY - gridHeight);
			glVertex2f((x + charPWidth) * xMult + rect.left,
					gridPHeight * yMult + rect.top);

			glTexCoord2f(cX, cY - gridHeight);
			glVertex2f(x * xMult + rect.left, gridPHeight * yMult + rect.top);

			x += charPWidth;
		}
		++i;
	}
	glEnd();
}
