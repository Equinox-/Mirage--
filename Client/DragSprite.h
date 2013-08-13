#ifndef _DRAGSPRITE_H
#define _DRAGSPRITE_H

#include <SGE.h>
using namespace SGE;

class DragSprite
{
public:
	DragSprite(SVector2 position, bool visible);
	~DragSprite();

	void Init(char* fileName);
	void Update(float deltaTime);
	void Render();

	void SetVisible(bool visible)							{ mVisible = visible; }
	void ToggleVisible()									{ mVisible = mVisible == true ? false : true; }
	int GetSpriteWidth()									{ return mSprite.GetWidth(); }
	int GetSpriteHeight()									{ return mSprite.GetHeight(); }
	
	bool GetVisible()										{ return mVisible; }
	SVector2 GetPosition()									{ return mPosition; }


private:
	SGE_Sprite mSprite;
	SVector2 mPosition;
	
	bool mVisible;
	bool mDragDown;
};

#endif