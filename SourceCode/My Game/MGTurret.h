/// \file MGTurret.h
/// \brief Interface for the MGTurret object class CMGTurret.

#ifndef __L4RC_GAME_MGTurret_H__
#define __L4RC_GAME_MGTurret_H__

#include "Object.h"

/// \brief The MGTurret object. 
///
/// CMGTurret is the abstract representation of a MGTurret object.

class CMGTurret : public CObject {
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
    CMGTurret(const Vector2& p); ///< Constructor.
    virtual void move(); ///< Move MGTurret.
    float m_fLastTimeFire = 0.0f;
}; //CBullet

#endif //__L4RC_GAME_MGTurret_H__
