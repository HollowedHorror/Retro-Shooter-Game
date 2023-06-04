/// \file Bullet.h
/// \brief Interface for the bullet object class CBullet.

#pragma once
#include "Object.h"

/// \brief The bullet object. 
///
/// The abstract representation of a bullet object. Bullet objects die in a
/// cloud of smoke when they collide with anything.

class CBullet: public CObject{
  protected:
    virtual void CollisionResponse(const Vector2&, float,
      CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.

  public:
    CBullet(eSprite t, const Vector2& p); ///< Constructor.
}; //CBullet
