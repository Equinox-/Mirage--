#include "HUD.h"

HUD::HUD(PlayerInfo& playerInfo, Raknet& raknet, Map& map, Character& character,
		ChatBox& chatBox, OtherPlayers& otherPlayers) :
		mEqiup(raknet, playerInfo, mInventory), mInventory(30, raknet, map,
				playerInfo, character), mHealthBar(1.14f), mManaBar(0.825f), mEnemyHealth(
				1.10f), mPlayerInfo(playerInfo), mRaknet(raknet), mChatBox(
				chatBox), mEnemySelected(0), mMap(map), mMiniMap(otherPlayers,
				map, character) {

}

HUD::~HUD() {

}

void HUD::Unload() {

}

void HUD::Init() {
	mInventory.Init();
	mMiniMap.Init();

	mHealthBar.Init(78, 26.5f, "HealthBar.png");
	mManaBar.Init(80, 53, "ManaBar.png");
	mEnemyHealth.Init(777, 40, "HealthBar.png");

	mBars.Load("GUI/Ingame/Health1.png");
	mCharWindow.Load("GUI/Ingame/Health.png");
	mSkills.Load("GUI/Ingame/Skill.png");
	mEnemyHUD.Load("GUI/Ingame/EnemyHUD.png");
	mEnemyHUD2.Load("GUI/Ingame/EnemyHUD1.png");
	mBars.SetPosition(65, 16);
	mCharWindow.SetPosition(5, 5);
	mSkills.SetPosition(975, 805);
	mEnemyHUD.SetPosition(700, 5);
	mEnemyHUD2.SetPosition(759, 30);
}

void HUD::InitEqiup() {
	mEqiup.Init();
}

void HUD::Update(float deltaTime) {
	mHealthBar.Update(deltaTime, mPlayerInfo.GetCurrentHealth(),
			mPlayerInfo.GetTotalHealth());
	mManaBar.Update(deltaTime, mPlayerInfo.GetCurrentMana(),
			mPlayerInfo.GetTotalMana());

	NPC* temp = mMap.GetNPC(mEnemySelected);

	if (temp) {
		NPCData& data = temp->GetData();
		mEnemyHealth.Update(deltaTime, data.mCurrentHealth, data.mTotalHealth);
	}
	mInventory.Update(deltaTime);
	mEqiup.Update(deltaTime);
	mMiniMap.Update(deltaTime);

	bool chatting = mChatBox.IsChatting();

	float x = Input_GetMouseScreenX();
	float y = Input_GetMouseScreenY();

	// Toggle visibility
	if (!chatting) {
		if (Input_IsKeyPressed(Keys::I)
				|| ((x > 1510) && (x < 1542) && (y > 831) && (y < 870)
						&& Input_IsMousePressed(Mouse::LBUTTON))) {
			mInventory.ToggleVisible();
		}

		if (Input_IsKeyPressed(Keys::C)
				|| ((x > 1542) && (x < 1580) && (y > 831) && (y < 870)
						&& Input_IsMousePressed(Mouse::LBUTTON))) {
			mEqiup.ToggleVisible();
		}
	}
}

void HUD::Render() {
	mInventory.Render();
	mBars.Render();
	mHealthBar.Render();
	mManaBar.Render();
	mEqiup.Render();
	mCharWindow.Render();
	mSkills.Render();
	mMiniMap.Render();

	if (mEnemySelected > 0) {
		mEnemyHUD2.Render();
		mEnemyHealth.Render();
		mEnemyHUD.Render();
	}

	int x = Input_GetMouseScreenX();
	int y = Input_GetMouseScreenY();

	char temp[CHAR_MAX];
	sprintf(temp, "Mouse X: %d", x);
	Graphics_DebugText(temp, 1200, 25, 0XFF0000);
	sprintf(temp, "Mouse Y: %d", y);
	Graphics_DebugText(temp, 1200, 50, 0XFF0000);
}

void HUD::UpdateRaknet() {
	mInventory.UpdateRaknet();
	mEqiup.UpdateRaknet();
}
