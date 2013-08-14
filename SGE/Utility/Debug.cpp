//====================================================================================================
// Filename:	Debug.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <vector>

#include "../Graphics/CDXGraphics.h"
#include "../Graphics/CSpriteRenderer.h"
#include "../Utility/Debug.h"
#include "../GLUtil/Math/Vector2f.h"
#include "../GLUtil/Math/Rectangle.h"
#include "../GLUtil/GLColor.h"
#include "../Graphics/CFont.h"
#include "../GLUtil/Math/MathConstants.h"
#include <math.h>
#include <time.h>

#define SHOW_USAGE_GRAPHIC 2
//====================================================================================================
// Namespace
//====================================================================================================

namespace Debug {

//====================================================================================================
// Structs
//====================================================================================================

struct SLine {
	Vector2f from;
	Vector2f to;
	GLColor rgb;
	float size;

	SLine(void) :
			from(0.0f, 0.0f), to(0.0f, 0.0f), rgb(0xFFFFFF), size(1.0f) {
	}

	SLine(const Vector2f& _from, const Vector2f& _to, GLColor _rgb, float _size) :
			from(_from), to(_to), rgb(_rgb), size(_size) {
	}
};

//----------------------------------------------------------------------------------------------------

struct SText {
	std::string text;
	Vector2f pos;
	GLColor rgb;

	SText(void) :
			text(""), pos(0.0f, 0.0f), rgb(0xFFFFFF) {
	}

	SText(const char* _text, float _x, float _y, GLColor _rgb) :
			text(_text), pos(_x, _y), rgb(_rgb) {
	}
};

struct SUsageNode {
	std::string name;
	float time;
	clock_t startTick;
	SUsageNode *parent;
	std::vector<SUsageNode*> children;
};

//====================================================================================================
// Statics
//====================================================================================================

static CFont* s_pFont = NULL;

static bool s_Initialized = false;

const int c_MaxNumLines = 10000;
static SLine s_LineList[c_MaxNumLines];

static int s_LineIndex = 0;

const int c_MaxText = 100;
static SText s_TextList[c_MaxText];

static int s_TextIndex = 0;
static float f_LineWidth_Debug = 1.0;

static char* sUsageBuff;

static SUsageNode *rootNode;
static SUsageNode *activeNode;

//====================================================================================================
// Function Definitions
//====================================================================================================
void clearUsageNode(SUsageNode *node) {
	for (size_t i = 0; i < node->children.size(); ++i) {
		clearUsageNode(node->children[i]);
	}
	delete node;
}
void initUsageTree() {
	rootNode = new SUsageNode();
	activeNode = rootNode;
	rootNode->time = 1000.0;
}
float drawUsageNode(SUsageNode *node, float x, float y) {
	sprintf(sUsageBuff, "%s: %f", node->name.c_str(), node->time);
	if (node->parent != NULL && node->parent->time >= 0.0
			&& node->parent != rootNode) {
		int percent = (int) (node->time / node->parent->time * 100.0);
		if (percent > 0 && percent <= 100) {
			sprintf(sUsageBuff, "%s: %d%% %f", node->name.c_str(), percent,
					node->time);
		}
	}
	sUsageBuff[49] = '\0';
	s_pFont->PrintText(sUsageBuff, x, y);
	y += 20.0;
	for (size_t i = 0; i < node->children.size(); ++i) {
		y = drawUsageNode(node->children[i], x + 10, y);
	}
	return y;
}

void Initialize(float fLineWidth) {
	// Check if we have initialized
	if (s_Initialized) {
		return;
	}
	f_LineWidth_Debug = fLineWidth;

	// Create a debug font
	//D3DXCreateFontA(CDXGraphics::Get()->D3DDevice(), 24, 0, 500, 1, 0,
	//		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
	//		DEFAULT_PITCH | FF_DONTCARE, "Impact", &s_pFont);
	s_pFont = new CFont();
	s_pFont->Create(FontType::ARIAL, 16);

#if SHOW_USAGE_GRAPHIC
	sUsageBuff = new char[50];
	initUsageTree();
#endif

	// Set flag
	s_Initialized = true;
}

//----------------------------------------------------------------------------------------------------

void Terminate(void) {
	// Check if we have terminated
	if (!s_Initialized) {
		return;
	}

	// Release everything
	if (NULL != s_pFont) {
		s_pFont->Destroy();
		s_pFont = NULL;
	}
	//if (NULL != s_pLineRenderer) {
	//	s_pLineRenderer->Release();
	//	s_pLineRenderer = NULL;
	//}

	// Set flag
	delete[] sUsageBuff;
	s_Initialized = false;
}

//----------------------------------------------------------------------------------------------------

void AddScreenLine(float x0, float y0, float x1, float y1, GLColor rgb) {
	// Check if we have initialized
	if (!s_Initialized) {
		return;
	}

	// Check if we have enough space
	if (s_LineIndex + 1 >= c_MaxNumLines) {
		return;
	}

	// Add line to our list for rendering later
	s_LineList[s_LineIndex++] = SLine(Vector2f(x0, y0), Vector2f(x1, y1), rgb,
			1.0f);
}

//----------------------------------------------------------------------------------------------------

void AddScreenRect(float x, float y, float w, float h, GLColor rgb) {
	// Check if we have initialized
	if (!s_Initialized) {
		return;
	}

	// Check if we have enough space
	if (s_LineIndex + 4 >= c_MaxNumLines) {
		return;
	}

	// Add line to our list for rendering later
	Vector2f A(x, y);
	Vector2f B(x + w, y);
	Vector2f C(x + w, y + h);
	Vector2f D(x, y + h);
	s_LineList[s_LineIndex++] = SLine(A, B, rgb, 1.0f);
	s_LineList[s_LineIndex++] = SLine(B, C, rgb, 1.0f);
	s_LineList[s_LineIndex++] = SLine(C, D, rgb, 1.0f);
	s_LineList[s_LineIndex++] = SLine(D, A, rgb, 1.0f);
}

//----------------------------------------------------------------------------------------------------

void AddScreenCircle(float x, float y, float r, GLColor rgb) {
	// Check if we have initialized
	if (!s_Initialized) {
		return;
	}

	// Check if we have enough space
	if (s_LineIndex + 16 >= c_MaxNumLines) {
		return;
	}

	// Add line to our list for rendering later
	const float angle = MATH_PI / 8.0f;
	for (int i = 0; i < 16; ++i) {
		float alpha(i * angle);
		float beta((i + 1) * angle);

		Vector2f A(r * sin(alpha) + x, r * cos(alpha) + y);
		Vector2f B(r * sin(beta) + x, r * cos(beta) + y);

		s_LineList[s_LineIndex++] = SLine(A, B, rgb, 1.0f);
	}
}

//----------------------------------------------------------------------------------------------------

void AddScreenText(const char* text, float x, float y, GLColor rgb) {
	// Check if we have initialized
	if (!s_Initialized) {
		return;
	}

	// Check if we have enough space
	if (s_TextIndex + 1 >= c_MaxText) {
		return;
	}

	// Add text to our list for rendering later
	s_TextList[s_TextIndex] = SText(text, x, y, rgb);
	++s_TextIndex;
}

//----------------------------------------------------------------------------------------------------
void Render(void) {
	// Check if we have initialized
	if (!s_Initialized) {
		return;
	}

	// Begin render
	glLineWidth(f_LineWidth_Debug);
	glBegin(GL_LINES);	//s_pLineRenderer->Begin();

	// Draw all the shapes
	for (int i = 0; i < s_LineIndex; ++i) {
		GLColor color = s_LineList[i].rgb | 0xFF000000;
		color.a = 1.0;
		//s_pLineRenderer->Draw(&s_LineList[i].from, 2, color);
		color.bind();
		glVertex2f(s_LineList[i].from.x, s_LineList[i].from.y);
		glVertex2f(s_LineList[i].to.x, s_LineList[i].to.y);
	}

	// End render
	glEnd();	// s_pLineRenderer->End();

	// Draw text
	for (int i = 0; i < s_TextIndex; ++i) {
		GLColor color = s_TextList[i].rgb | 0xFF000000;
		//s_pFont->DrawTextA(NULL, s_TextList[i].text.c_str(), -1, &rect, 0,
		//		color);
		s_pFont->PrintText(s_TextList[i].text.c_str(), s_TextList[i].pos.x,
				s_TextList[i].pos.y);
	}

	// Reset index
	s_LineIndex = 0;
	s_TextIndex = 0;

#if SHOW_USAGE_GRAPHIC
	// Just 1 layer
	float x = 200.0;
	float y = 25.0;
	for (size_t i = 0; i < rootNode->children.size(); ++i) {
		y = drawUsageNode(rootNode->children[i], x, y);
	}
	clearUsageNode(rootNode);
	initUsageTree();
#endif
}

void UsageBegin(const char* name) {
#if SHOW_USAGE_GRAPHIC
	SUsageNode *node = new SUsageNode();
	activeNode->children.push_back(node);
	node->parent = activeNode;
	node->name = name;
	node->startTick = clock();
	activeNode = node;
#endif
}
void UsageEnd() {
#if SHOW_USAGE_GRAPHIC
	activeNode->time = (float) (clock() - activeNode->startTick)
			/ (float) CLOCKS_PER_SEC;
	activeNode = activeNode->parent;
#endif
}

} // namespace Debug
