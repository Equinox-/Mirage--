/*
 * GLTexture.h
 *
 *  Created on: Jul 31, 2013
 *      Author: lwestin
 */

#ifndef GLTEXTURE_H_
#define GLTEXTURE_H_

#include <stdio.h>
#include "GLColor.h"

#define TEXTURE_LOAD_DEBUGGING	(0)
#define TEXTURE_LOAD_SUCCESS	(0)
#define TEXTURE_LOAD_ERROR		(1)

typedef unsigned int texid_t;


class GLTexture {
private:
	texid_t textureID;
	void *rawData;
	unsigned int width;
	unsigned int height;
	unsigned int pixFMT;

	// Information to access the data
	unsigned int rowBytes;
	unsigned int bitsPerPixel;
public:
	GLTexture();
	virtual ~GLTexture();

	int loadFromPNG(FILE *fp);
	int loadFromFile(const char *fname);
	void freeRawData();
	void freeTexture();

	void loadToVRAM();
	void bind();

	int getWidth();
	int getHeight();

	GLColor getColorAt(int x, int y);
};

#endif /* TEXTURE_H_ */
