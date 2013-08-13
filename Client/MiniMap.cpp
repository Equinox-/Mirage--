#include "MiniMap.h"

MiniMap::MiniMap(OtherPlayers& otherPlayers, Map& map, Character& character)
:mOtherPlayers(otherPlayers)
,mMap(map)
,mCharacter(character)
{

}

MiniMap::~MiniMap()
{

}

void MiniMap::Init()
{
	mMiniMap.Load("GUI/Ingame/MiniMap.png");
	mMiniMapChar.Load("GUI/Ingame/MiniMapChar.png");
	mMiniMapEnemy.Load("GUI/Ingame/EnemyDot.png");
	mMiniMapPlayer.Load("GUI/Ingame/PlayerDot.png");

	mMiniMap.SetPosition(1375, 10);
	mMiniMapChar.SetPosition(1435, 35);

	mMiniMapChar.SetRotation(0.2);
	mMiniMapChar.SetScaleAboutCenter(false);
}

void MiniMap::Update(float deltaTime)
{

}

void MiniMap::Render()
{
	mMiniMap.Render();

	SVector2 boxPosition(1375, 10);
	int numberOfNPCS = mMap.GetNumberOfNPCS();
	int numberOfPlayers = mOtherPlayers.GetNumberOfPlayers();

	SCircle mapCircle(1482, 114, 75);
	//Graphics_DebugCircle(mapCircle, 0XFF0000);


	SVector2 offset(boxPosition.x + mMiniMap.GetWidth() * 0.5f, boxPosition.y + mMiniMap.GetHeight() * 0.5f);

	for(int a = 0; a < numberOfNPCS; ++a)
	{
		SVector2 position = ((mMap.GetNPC(a)->GetPosition() - mCharacter.GetPosition()) * 0.05f + offset);

		SCircle tempCircle(position, 1);

		// If within our map radius
		if(Intersect(tempCircle, mapCircle))
		{
			mMiniMapEnemy.SetPosition(position);
			mMiniMapEnemy.Render();
		}
	}

	// If within our map radius
	mMiniMapPlayer.SetPosition(offset);
	mMiniMapPlayer.Render();

	for(int a = 0; a < numberOfPlayers; ++a)
	{
		SVector2 position = ((mOtherPlayers.GetPlayer(a)->GetPosition()  - mCharacter.GetPosition()) * 0.05f + offset);
		SCircle tempCircle(position, 1);

		// If within our map radius
		if(Intersect(tempCircle, mapCircle))
		{
			mMiniMapChar.SetPosition(position);
			mMiniMapChar.Render();
		}
	}


}