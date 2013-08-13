#include "NPCHelper.h"

NPCData* GetNPC(int number) {
	int npcType = 0;
	char temp[CHAR_MAX];
	sprintf(temp, "NPCS/%d.npc", number);
	FILE* pFile = fopen(temp, "rb");

	fscanf(pFile, "%d", &npcType);

	NPCData* pData = new NPCData;
	fread(pData, 1, sizeof(NPCData), pFile);

	return pData;
}
