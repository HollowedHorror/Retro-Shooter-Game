/// \file Ghost.h
/// \brief Interface for the ant object class CGhost.

#ifndef __L4RC_GAME_GHOST_H__
#define __L4RC_GAME_GHOST_H__

#include "Object.h"
#include "Common.h"

/// \brief The ant object. 
///
/// CGhost is the abstract representation of an ant object.

class CGhost : public CObject {
    friend class CObjectManager; ///< Object manager is a friend.

protected:
    const UINT m_nMaxHealth = 1; ///<Maximum health.
    UINT m_nHealth = m_nMaxHealth; ///<Current health.
    int bulletSkip = 1; //Integer to skip a bullet one time to avoid death on spawn.
    bool stopMoving = false; //Bool to detect if the ghost is colliding with player.

    void Follow(const Vector2&); ///< Swivel towards position.
    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.

public:
    CGhost(const Vector2&); ///< Constructor. 
    ~CGhost(); ///< Destructor. 

    virtual void move(); ///< Move Ghost.
}; //CGhost

#endif //__L4RC_GAME_GHOST_H__
