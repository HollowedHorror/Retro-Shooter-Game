/// \file ObjectManager.h
/// \brief Interface for the object manager CObjectManager.

#ifndef __L4RC_GAME_OBJECTMANAGER_H__
#define __L4RC_GAME_OBJECTMANAGER_H__

#include "BaseObjectManager.h"
#include "Object.h"
#include "Common.h"

/// \brief The object manager.
///
/// A collection of all of the game objects.

class CObjectManager: 
  public LBaseObjectManager<CObject>,
  public CCommon
{
  private:
    //bool m_bLevelCompleted = false; ///< Level completion flag.

    void BroadPhase(); ///< Broad phase collision detection and response.
    void NarrowPhase(CObject*, CObject*); ///< Narrow phase collision detection and response.

  public:
    //virtual void clear(); ///< Reset to empty and delete all objects.

    CObject* create(eSprite, const Vector2&); ///< Create new object.
    
    virtual void draw(); ///< Draw all objects.

    void FireGun(CObject*, eSprite); ///< Fire object's gun.
    const size_t GetNumEnemies() const; ///< Get number of turrets in object list.
    const int maxGhosts = 3;
    int numOfGhosts = 0;
    //const bool LevelCompleted() const; ///< Level completed.

    //Difficulty Variables
    UINT m_nDifficultyModifier = 0; //this stat is used to determine outcomes based on the game's difficulty

}; //CObjectManager

#endif //__L4RC_GAME_OBJECTMANAGER_H__
