//====================================================================================================
// Filename:	CWireFrame.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "CWireFrame.h"
#include <GL/gl.h>
#include "../Core/CLog.h"
#include "../Graphics/CDXGraphics.h"
#include <math.h>
#include "../GLUtil/GLColor.h"
#include "../GLUtil/Math/Vector2f.h"
#include "../GLUtil/Math/MathConstants.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

CWireFrame::CWireFrame(void) :
		mVertices(), lineWidth(0.0), mColor(0xFFFFFFFF) {
	// Empty
}

//----------------------------------------------------------------------------------------------------

CWireFrame::CWireFrame(const CWireFrame& rhs) :
		mVertices() {
	// Copy vertex data
	const int kNumVertices = rhs.mVertices.size();
	for (int i = 0; i < kNumVertices; ++i) {
		mVertices.push_back(rhs.mVertices[i]);
	}
	lineWidth = rhs.lineWidth;
	// Copy color
	mColor = rhs.mColor;
}

//----------------------------------------------------------------------------------------------------

CWireFrame::~CWireFrame(void) {
	// Release everything
	Destroy();
}

//----------------------------------------------------------------------------------------------------

void CWireFrame::Create(void) {
	// Release everything
	Destroy();

	// Create a new ID3DXLine interface for rendering
	//D3DXCreateLine(CDXGraphics::Get()->D3DDevice(), &mpLineRenderer);
}

//----------------------------------------------------------------------------------------------------

void CWireFrame::Destroy(void) {
}

//----------------------------------------------------------------------------------------------------

void CWireFrame::Render(void) {
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	// Draw all the shapes
	mColor.bind();
	for (size_t i = 0; i < mVertices.size(); ++i) {
		glVertex2f(mVertices[i].x, mVertices[i].y);
	}
	glEnd();
}

//----------------------------------------------------------------------------------------------------

void CWireFrame::AddLine(float x0, float y0, float x1, float y1) {
	mVertices.push_back(Vector2f(x0, y0));
	mVertices.push_back(Vector2f(x1, y1));
}

//----------------------------------------------------------------------------------------------------

void CWireFrame::AddRect(float x, float y, float w, float h) {
	mVertices.push_back(Vector2f(x, y));
	mVertices.push_back(Vector2f(x + w, y));
	mVertices.push_back(Vector2f(x + w, y));
	mVertices.push_back(Vector2f(x + w, y + h));
	mVertices.push_back(Vector2f(x + w, y + h));
	mVertices.push_back(Vector2f(x, y + h));
	mVertices.push_back(Vector2f(x, y + h));
	mVertices.push_back(Vector2f(x, y));
}

//----------------------------------------------------------------------------------------------------

void CWireFrame::AddCircle(float x, float y, float r) {
	// Add line to our list for rendering later
	const float angle = MATH_PI / 8.0f;
	for (int i = 0; i < 16; ++i) {
		const float alpha(i * angle);
		const float beta((i + 1) * angle);

		mVertices.push_back(Vector2f(r * sin(alpha) + x, r * cos(alpha) + y));
		mVertices.push_back(Vector2f(r * sin(beta) + x, r * cos(beta) + y));
	}
}

//----------------------------------------------------------------------------------------------------

void CWireFrame::Purge(void) {
	mVertices.clear();
}

//----------------------------------------------------------------------------------------------------

void CWireFrame::SetLineWidth(float fLineWidth) {
	lineWidth = fLineWidth;
}

//----------------------------------------------------------------------------------------------------

void CWireFrame::SetColor(GLColor rgb) {
	mColor = rgb | 0xFF000000;
}
