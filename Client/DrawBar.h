#ifndef _DRAWBAR_H
#define _DRAWBAR_H

#include <SGE.h>
using namespace SGE;

class DrawBar
{
public:
	DrawBar(float length);
	~DrawBar();

	void Init(float x, float y, char* fileName);
	void Update(float deltaTime, float currentValue, float totalValue);
	void Render();

private:
	SGE_Sprite mSpriteCurrent;
	float mLength;
};

#endif