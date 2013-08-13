#include "MapData.h"
#include "NPCHelper.h"

MapData::MapData() :
		mLayer1(NULL), mLayer2(NULL), mLayer3(NULL), mLayer4(NULL), mWidth(0), mHeight(
				0), mTileAmount(0), mMapNumber(0), mWeatherIntensity(0), mPlayers(
				0), mNumberOfNPCS(0), mNorth(0), mEast(0), mSouth(0), mWest(0) {

}

MapData::~MapData() {
	if (mLayer1) {
		delete[] mLayer1;
	}
	if (mLayer2) {
		delete[] mLayer2;
	}
	if (mLayer3) {
		delete[] mLayer3;
	}
	if (mLayer4) {
		delete[] mLayer4;
	}

	for (int a = 0; a < mNumberOfNPCS; ++a) {
		delete mNPCS[a];
	}
}

void MapData::CreateTiles() {
	if (mWidth > 0 && mHeight > 0) {
		mTileAmount = ((mWidth / 32) + 1) * ((mHeight / 32) + 1);
		mLayer1 = new GroundTile[mTileAmount];
		mLayer2 = new Tile[mTileAmount];
		mLayer3 = new Tile[mTileAmount];
		mLayer4 = new Tile[mTileAmount];
	}
}

void MapData::CreateNPCS() {
	mNumberOfNPCS = 0;

	for (int a = 0; a < mTileAmount; ++a) {
		int npc = mLayer1[a].GetNPC();

		if (npc > 0) {
			++mNumberOfNPCS;

			NPCData* temp;
			temp = GetNPC(npc);
			temp->SetPosition(mLayer1[a].GetPosition());
			mNPCS.push_back(temp);
		}
	}
}

Tile* MapData::GetLayer(int layer) {
	switch (layer) {
	case 1: {
		return mLayer1;
	}
	case 2: {
		return mLayer2;
	}
	case 3: {
		return mLayer3;
	}
	case 4: {
		return mLayer4;
	}
	default: {

		break;
	}
	}
}

void MapData::SetTileInfo(float x, float y, int tile, int layer, int id) {
	Tile* temp;
	switch (layer) {
	case 1: {
		temp = &mLayer1[tile];
		break;
	}
	case 2: {
		temp = &mLayer2[tile];
		break;
	}
	case 3: {
		temp = &mLayer3[tile];
		break;
	}
	case 4: {
		temp = &mLayer4[tile];
		break;
	}
	default: {

		break;
	}
	}
	temp->SetPosition(SVector2(x, y));
	temp->SetType(id);
}

void MapData::SendInfo(RakNet::BitStream& bsOut, Packet* packet) {
	int npcCount = 0;
	for (int a = 0; a < mTileAmount; ++a) {
		bsOut.Write(mLayer1[a]);
		bsOut.Write(mLayer2[a]);
		bsOut.Write(mLayer3[a]);
		bsOut.Write(mLayer4[a]);

		int npc = mLayer1[a].GetNPC();

		if (npc > 0) {
			mNPCS[npcCount]->SetHost(false);

			// If only player on the map
			if (mPlayers.size() == 1) {
				mNPCS[npcCount]->SetHost(true);
				mHostGUID = packet->guid;
			}

			++npcCount;
		}
	}

	bsOut.Write(mNumberOfNPCS);

	for (int a = 0; a < mNumberOfNPCS; ++a) {
		bsOut.Write(*mNPCS[a]);
	}

	bsOut.Write(mNorth);
	bsOut.Write(mEast);
	bsOut.Write(mSouth);
	bsOut.Write(mWest);
}

GroundTile* MapData::FindItem(int id) {
	for (int a = 0; a < mTileAmount; ++a) {
		if (mLayer1[a].GetItemID() == id) {
			return &mLayer1[a];
		}
	}
}

void MapData::AddItem(int imageNumber, int itemNumber, int index) {
	mLayer1[index].SetItemImageNumber(imageNumber);
	mLayer1[index].SetItemNumber(itemNumber);
	mLayer1[index].SetIndex(index);
	mLayer1[index].SetItemID(index);
}

void MapData::PlayerDisconnect(RakNetGUID& guid, RakPeerInterface* peer) {
	RemovePlayer(guid);

	// NPC Host left better find a new one
	if (guid == mHostGUID && mPlayers.size() > 0) {
		mHostGUID = *mPlayers.begin();

		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID) ID_NPC_HOST);
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, mHostGUID,
				false);
	}
}

void MapData::AddPlayer(RakNetGUID& guid) {
	mPlayers.push_back(guid);
}

void MapData::RemovePlayer(RakNetGUID& guid) {
	for (list<RakNetGUID>::iterator iter = mPlayers.begin();
			iter != mPlayers.end(); ++iter) {
		if ((*iter) == guid) {
			iter = mPlayers.erase(iter);
			return;
		}
	}
}
