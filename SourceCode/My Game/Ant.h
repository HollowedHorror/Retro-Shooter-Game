/// \file Ant.h
/// \brief Interface for the ant object class CAnt.

#ifndef __L4RC_GAME_ANT_H__
#define __L4RC_GAME_ANT_H__

#include "Object.h"
#include "Common.h"

/// \brief The ant object. 
///
/// CAnt is the abstract representation of an ant object.

class CAnt: public CObject{
  friend class CObjectManager; ///< Object manager is a friend.

  protected: 
    const UINT m_nMaxHealth = 1; ///<Maximum health.
    UINT m_nHealth = m_nMaxHealth; ///<Current health.

    LEventTimer* m_pFrameEvent = nullptr; ///< Frame event timer.
    
    LEventTimer* m_pStrayEvent = nullptr; ///< Stray event timer.
    bool m_bStrayParity = true; ///< Stray from path left or right.

    bool m_bPreferPosRot = true; ///< Prefer positive rotation.

    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.

    void StrayFromPath(); ///< Stray randomly from path.
    void UpdateFramenumber(); ///< Update frame number.

  public:
    CAnt(const Vector2&); ///< Constructor. 
    ~CAnt(); ///< Destructor. 

    virtual void move(); ///< Move ant.
}; //CAnt

#endif //__L4RC_GAME_ANT_H__