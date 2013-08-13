#ifndef _HUD_H
#define _HUD_H

#include <SGE.h>
#include "Equip.h"
#include "MiniMap.h"
#include "DrawBar.h"
#include "Inventory.h"
#include "ChatBox.h"
#include "Raknet.h"
#include "PlayerInfo.h"
#include "ChatBox.h"
#include "OtherPlayers.h"

using namespace SGE;

class HUD
{
public:
	HUD(PlayerInfo& playerInfo, Raknet& raknet, Map& map, Character& character, ChatBox& chatBox, OtherPlayers& otherPlayers);
	~HUD();

	void Init();
	void InitEqiup();
	void Update(float deltaTime);
	void UpdateRaknet();
	void Render();
	void Unload();

private:

	SGE_Sprite mBars;
	SGE_Sprite mCharWindow;
	SGE_Sprite mSkills;
	SGE_Sprite mEnemyHUD;
	SGE_Sprite mEnemyHUD2;

	DrawBar mHealthBar;
	DrawBar mManaBar;
	DrawBar mEnemyHealth;
	MiniMap mMiniMap;
	Inventory mInventory;
	Equip mEqiup;
	PlayerInfo& mPlayerInfo;
	Raknet& mRaknet;
	Map& mMap;
	ChatBox& mChatBox;

	int mEnemySelected;
};
#endif
