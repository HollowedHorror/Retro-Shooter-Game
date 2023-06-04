/// \file Object.h
/// \brief Interface for the game object class CObject.

#ifndef __L4RC_GAME_OBJECT_H__
#define __L4RC_GAME_OBJECT_H__

#include "GameDefines.h"
#include "SpriteRenderer.h"
#include "Common.h"
#include "Component.h"
#include "SpriteDesc.h"
#include "BaseObject.h"
#include "EventTimer.h"

/// \brief The game object. 
///
/// The abstract representation of an object. `CObjectManager` is a friend of
/// this class so that it can access any private members as needed to manage
/// the objects without the need for reader and set functions for each private
/// or protected member variable. This class must contain public member
/// functions `move()` and `draw()` to move and draw the object, respectively.

class CObject:
  public CCommon,
  public LBaseObject
{
  friend class CObjectManager; ///< Object manager needs access so it can manage.

  protected:
    float m_fRadius = 0; ///< Bounding circle radius.

    float m_fSpeed = 0; ///< Speed.
    float m_fRotSpeed = 0; ///< Rotational speed.
    Vector2 m_vVelocity; ///< Velocity.


    bool m_bStatic = true; ///< Is static (does not move).
    bool m_bIsTarget = true; ///< Is a target.

    //bullet object bools
    bool m_bIsBullet = false; ///< Is a bullet.
    bool m_bIsPlayerBullet = false; // used to specify that the bullet is the player's bullet
    bool m_bIsEnemyBullet = false; // used to specify that the bullet is an enemy's bullet

    //powerup object bools
    bool m_bIsPowerUp = false; // a powerup
    bool m_bIsHealth = false; //health pickup
    bool m_bIsHealthUp = false; //increase max Health pickup
    bool m_bIsStaminaUp = false;  //increase max Stamina pickup
    bool m_bIsFocusUp = false;    //increase max Focus pickup
    bool m_bIsMovementSpeedUp = false;  //increase Movement Speed pickup
    bool m_bIsDamageUp = false; //increase Damage pickup

    //creature object bools
    bool m_bIsPlayer = false; ///< Is the player.
    bool m_bIsAnt = false; ///< Is an ant.
    bool m_bIsTurret = false; // Is a turret
    bool m_bIsAnimalControlOfficer = false; ///< Is the animal control officer.
    bool m_bIsBossTurret = false; ///< Is the boss turret.
    bool m_bIsGhost = false; ///< Is a ghost.

    LEventTimer* m_pGunFireEvent = nullptr; ///< Gun fire event.
    
    virtual void CollisionResponse(const Vector2&, float,
      CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.

    const Vector2 GetViewVector() const; ///< Compute view vector.

  public:
    CObject(eSprite, const Vector2&); ///< Constructor.
    virtual ~CObject(); ///< Destructor.

    void move(); ///< Move object.
    void draw(); ///< Draw object.

    //bullet bool functions
    const bool isBullet() const; ///< Is a bullet.
    const bool isPlayerBullet() const; // Is a player's bullet.
    const bool isEnemyBullet() const; // Is an enemy bullet.

    //powerup bool functions
    const bool isPowerUp() const; //is a powerup
    const bool isHealth() const;    //health pickup
    const bool isHealthUp() const;  //increase max Health pickup
    const bool isStaminaUp() const; //increase max Stamina pickup
    const bool isFocusUp() const;   //increase max Focus pickup
    const bool isMovementSpeedUp() const;   //increase Movement Speed pickup
    const bool isDamageUp() const;  //increase Damage pickup

    //creature bool functions
    const bool isPlayer() const; ///< Is the player.
    const bool isAnt() const; ///< Is an ant.
    const bool isTurret() const; // Is a turret
    const bool isAnimalControlOfficer() const; ///< Is the animal control officer.
    const bool isBossTurret() const; ///< Is the boss turret.
    const bool isGhost() const; ///< Is a ghost.
}; //CObject
#endif __L4RC_GAME_OBJECT_H__