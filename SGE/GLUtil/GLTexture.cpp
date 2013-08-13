/*
 * GLTexture.cpp
 *
 *  Created on: Jul 31, 2013
 *      Author: lwestin
 */

#include "GLTexture.h"
#include "GLColor.h"
#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <GL/gl.h>

GLTexture::GLTexture() {
	pixFMT = 0;
	rawData = NULL;
	height = 0;
	width = 0;
	textureID = 0;
	rowBytes = 0;
	bitsPerPixel = 0;
}

GLTexture::~GLTexture() {
	freeRawData();
	freeTexture();
}

void GLTexture::freeRawData() {
	if (rawData != NULL) {
		free(rawData);
		rawData = NULL;
	}
}

void GLTexture::freeTexture() {
	if (textureID > 0) {
		glDeleteTextures(1, &textureID);
	}
}

void GLTexture::loadToVRAM() {
	// If we are bound....
	if (textureID == 0) {
		freeTexture();
		if (rawData != NULL) {
			glGenTextures(1, &textureID);  //Grab a texture ID
			bind();
			glTexImage2D(GL_TEXTURE_2D, 0, pixFMT, width, height, 0, pixFMT,
					GL_UNSIGNED_BYTE, (GLvoid*) rawData);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}
}

static texid_t currentTexture = 0;
void GLTexture::bind() {
	if (rawData != NULL && textureID <= 0) {
		loadToVRAM();
	}
	if (currentTexture != textureID && textureID > 0) {
		glBindTexture(GL_TEXTURE_2D, textureID);
		currentTexture = textureID;
	}
}

int GLTexture::loadFromPNG(FILE *fp) {
	//header for testing if it is a png
	png_byte header[8];

	//read the header
	fread(header, 1, 8, fp);

	//test if png
	int is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png) {
		return TEXTURE_LOAD_ERROR;
	}

	//create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
			NULL, NULL);
	if (!png_ptr) {
		return TEXTURE_LOAD_ERROR;
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		return TEXTURE_LOAD_ERROR;
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		return TEXTURE_LOAD_ERROR;
	}

	//png load error jump position
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		return TEXTURE_LOAD_ERROR;
	}

	//init png reading
	png_init_io(png_ptr, fp);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 twidth, theight;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
			NULL, NULL, NULL);
	bitsPerPixel = bit_depth * png_get_channels(png_ptr, info_ptr);

	//update width and height based on png info
	width = twidth;
	height = theight;
#if TEXTURE_LOAD_DEBUGGING
	printf("PNG:\tLoaded %dx%d with a depth of %d and color type of ", width,
			height, bit_depth);
#endif

	//Update color info
	switch (color_type) {
	case PNG_COLOR_TYPE_GRAY:
		pixFMT = GL_LUMINANCE;
#if TEXTURE_LOAD_DEBUGGING
		printf("GL_LUMINANCE\n");
#endif
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		pixFMT = GL_LUMINANCE_ALPHA;
#if TEXTURE_LOAD_DEBUGGING
		printf("GL_LUMINANCE_ALPHA\n");
#endif
		break;
	case PNG_COLOR_TYPE_RGBA:
		pixFMT = GL_RGBA;
#if TEXTURE_LOAD_DEBUGGING
		printf("GL_ARGB\n");
#endif
		break;
	case PNG_COLOR_TYPE_RGB:
		pixFMT = GL_RGB;
#if TEXTURE_LOAD_DEBUGGING
		printf("GL_RGB\n");
#endif
		break;
	default:
		printf("\n");
		fprintf(stderr, "PNG:\tUnsupported pixformat: %d\n", color_type);
		return TEXTURE_LOAD_ERROR;
	}

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	rowBytes = png_get_rowbytes(png_ptr, info_ptr);

	// Allocate the image_data as a big block, to be given to opengl
	rawData = malloc(rowBytes * height);
	if (!rawData) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		return TEXTURE_LOAD_ERROR;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = new png_bytep[height];
	if (!row_pointers) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		return TEXTURE_LOAD_ERROR;
	}
	// set the individual row_pointers to point at the correct offsets of image_data
	for (unsigned register int i = 0; i < height; ++i) {
		row_pointers[height - 1 - i] = (png_bytep) rawData + i * rowBytes;
	}

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	//Now generate the OpenGL texture object
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] row_pointers;
	return TEXTURE_LOAD_SUCCESS;
}

GLColor GLTexture::getColorAt(int x, int y) {
	GLColor c;
	char* ptr = ((char*) rawData)
			+ (rowBytes * (height - y - 1) + (x * bitsPerPixel / 8));
	switch (pixFMT) {
	case GL_RGBA:
		c.setValue(
				((ptr[0] & 0xff) << 24) | ((ptr[1] & 0xff) << 16)
						| ((ptr[2] & 0xff) << 8) | (ptr[3] & 0xff));
		break;
	case GL_RGB:
		c.setValue(
				((ptr[0] & 0xff) << 24) | ((ptr[1] & 0xff) << 16)
						| ((ptr[2] & 0xff) << 8) | 0xff);
		break;
	case GL_LUMINANCE:
		c.setValue(
				((ptr[0] & 0xff) << 24) | ((ptr[0] & 0xff) << 16)
						| ((ptr[0] & 0xff) << 8) | 0xff);
		break;
	case GL_LUMINANCE_ALPHA:
		c.setValue(0xffffffff & (ptr[0] & 0xff));
		break;
	}
	return c;
}

int GLTexture::getWidth() {
	return width;
}

int GLTexture::getHeight() {
	return height;
}

int GLTexture::loadFromFile(const char *fname) {
	FILE *fp = fopen(fname, "rb");  // Open for binary reading
	if (!fp) {
		fprintf(stderr, "Unable to open file %s\n", fname);
		return TEXTURE_LOAD_ERROR;
	}
	// Grab the extension
	int status = loadFromPNG(fp);
	fclose(fp);
	return status;
}
