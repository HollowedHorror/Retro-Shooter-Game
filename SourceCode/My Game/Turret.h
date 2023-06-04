/// \file Turret.h
/// \brief Interface for the turret object class CTurret.

#ifndef __L4RC_GAME_TURRET_H__
#define __L4RC_GAME_TURRET_H__

#include "Object.h"

/// \brief The turret object. 
///
/// CTurret is the abstract representation of a turret object.

class CTurret: public CObject{
  protected:
    const UINT m_nMaxHealth = 30; ///< Maximum health.
    UINT m_nHealth = m_nMaxHealth; ///< Current health.

    float timeElapsed = 0;
    int random = 0;
    bool scan = true;
    float high = 0.6;//max scan turn speed
    float low = 0.2;//min scan turn speed
    
    void RotateTowards(const Vector2&); ///< Swivel towards position.
    void RandomScan();
    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.

  public:
    CTurret(const Vector2& p); ///< Constructor.
    virtual void move(); ///< Move turret.
}; //CBullet

#endif //__L4RC_GAME_TURRET_H__
