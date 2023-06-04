/// \file BossTurret.h
/// \brief Interface for the turret object class CBossTurret.

#ifndef __L4RC_GAME_BOSSTURRET_H__
#define __L4RC_GAME_BOSSTURRET_H__

#include "Object.h"

/// \brief The turret object. 
///
/// CBossTurret is the abstract representation of a turret object.

class CBossTurret : public CObject {
protected:
    const UINT m_nMaxHealth = 90; ///< Maximum health.
    UINT m_nHealth = m_nMaxHealth; ///< Current health.
    int stage = 1;
    int movesetChoice;

    float timeElapsed = 0;
    int random = 0;
    bool scan = true;
    float high = 0.6;//max scan turn speed
    float low = 0.2;//min scan turn speed

    void RotateTowards(const Vector2&); ///< Swivel towards position.
    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.

public:
    CBossTurret(const Vector2& p); ///< Constructor.
    virtual void move(); ///< Move turret.
    float m_fTimeLastFire = 0.0f;
    float m_fTimeLastSwitch = 0.0f;
}; //CBullet

#endif //__L4RC_GAME_BOSSTURRET_H__
