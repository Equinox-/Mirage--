#ifndef INCLUDED_CHARACTERSTATE_H
#define INCLUDED_CHARACTERSTATE_H

//====================================================================================================
// Includes
//====================================================================================================

#include <SGE.h>
using namespace SGE;
#include "Animation.h"
#include "PlayerInfo.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Character;
class Animation;
//====================================================================================================
// Class Declarations
//====================================================================================================

class CharacterState
{
public:
    CharacterState(Character* pOwner, const char* name, PlayerInfo& playerInfo, Raknet& raknet) : mpOwner(pOwner), mName(name), mPlayerInfo(playerInfo), mRaknet(raknet) {}
    virtual ~CharacterState() {};

    virtual void Load() = 0;
    virtual void Unload() = 0;
    virtual bool Update(float fSeconds) = 0;
    virtual void Render(const SVector2& offset) = 0;

    virtual void Enter() = 0;
    virtual void Exit() = 0;

    virtual SRect GetBoundingBox() const = 0;

    const char* GetName() const								{ return mName; }

protected:
    SGE_Sprite mSprite;
	Animation mAnimation;
    Character* mpOwner;
    const char* mName;
	PlayerInfo& mPlayerInfo;
	Raknet& mRaknet;
};

#endif