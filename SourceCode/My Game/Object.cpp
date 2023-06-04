/// \file Object.cpp
/// \brief Code for the game object class CObject.

#include "Object.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "Helpers.h"

/// Create and initialize an object given its sprite type and initial position.
/// \param t Type of sprite.
/// \param p Initial position of object.

CObject::CObject(eSprite t, const Vector2& p):
  LBaseObject(t, p)
{ 
  if (t != eSprite::Door)m_fRoll = XM_PIDIV2; //facing upwards everything except for doors
  m_bIsTarget = false; //not a target

  const float w = m_pRenderer->GetWidth(t); //sprite width
  const float h = m_pRenderer->GetHeight(t); //sprite height
  m_fRadius = std::max(w, h)/2; //bounding circle radius

  m_pGunFireEvent = new LEventTimer(1.0f); //timer for firing gun
} //constructor

/// Destructor.

CObject::~CObject(){
  delete m_pGunFireEvent;
} //destructor

/// Move object an amount that depends on its velocity and the frame time.

void CObject::move(){
  if(!m_bDead && !m_bStatic)
    m_vPos += m_vVelocity*m_pTimer->GetFrameTime();
} //move

/// Ask the renderer to draw the sprite described in the sprite descriptor.
/// Note that `CObject` is derived from `LBaseObject` which is inherited from
/// `LSpriteDesc2D`. Therefore `LSpriteRenderer::Draw(const LSpriteDesc2D*)`
/// will accept `this` as a parameter, automatically down-casting it from
/// `CObject*` to `LSpriteDesc2D*`, effectively drawing the object from its
/// sprite descriptor.

void CObject::draw(){ 
  m_pRenderer->Draw(this);
} //draw

/// Response to collision. Move back the overlap distance along the collision
/// normal. 
/// \param norm Collision normal.
/// \param d Overlap distance.
/// \param pObj Pointer to object being collided with (defaults to `nullptr`,
/// which means collision with a wall).

void CObject::CollisionResponse(const Vector2& norm, float d, CObject* pObj){
  if(m_bDead)return; //dead, bail out

  const Vector2 vOverlap = d*norm; //overlap in direction of this
  const bool bStatic = !pObj || pObj->m_bStatic; //whether other object is static

  if(!m_bStatic && !bStatic) //both objects are dynamic
        m_vPos += vOverlap/2; //back off this object by half

  else if(!m_bStatic && bStatic) //only this object is dynamic
        m_vPos += vOverlap; //back off this object
} //CollisionResponse

/// Create a particle effect to mark the death of the object.
/// This function is a stub intended to be overridden by various object classes
/// derived from this class.

void CObject::DeathFX(){
 //stub
} //DeathFX

/// Compute the view vector from the object orientation.
/// \return The view vector.

const Vector2 CObject::GetViewVector() const
{
    return AngleToVector(m_fRoll);
} //ViewVector

// Reader function for PowerUp flags.
// is a powerup
const bool CObject::isPowerUp() const
{
    return m_bIsPowerUp;
}

// For health pickup
const bool CObject::isHealth() const 
{
    return m_bIsHealth;
}

//increase max Health pickup
const bool CObject::isHealthUp() const
{
    return m_bIsHealthUp;
}

//increase max Stamina pickup
const bool CObject::isStaminaUp() const
{
    return m_bIsStaminaUp;
}

//increase max Focus pickup
const bool CObject::isFocusUp() const
{
    return m_bIsFocusUp;
}

//increase Movement Speed pickup
const bool CObject::isMovementSpeedUp() const
{
    return m_bIsMovementSpeedUp;
}

//increase Damage pickup
const bool CObject::isDamageUp() const
{
    return m_bIsDamageUp;
}

/// Reader function for bullet flags.
/// \return true if a bullet.
const bool CObject::isBullet() const
{
    return m_bIsBullet;
} //isBullet

const bool CObject::isPlayerBullet() const 
{
    return m_bIsPlayerBullet;
} //isPlayerBullet

const bool CObject::isEnemyBullet() const
{
    return m_bIsEnemyBullet;
} //isEnemyBullet

// Reader function for creature flags.
const bool CObject::isPlayer() const 
{
    return m_bIsPlayer;
} //isPlayer

const bool CObject::isAnt() const 
{
    return m_bIsAnt;
} //isAnt

const bool CObject::isTurret() const
{
    return m_bIsTurret;
} //isTurret

const bool CObject::isAnimalControlOfficer() const 
{
    return m_bIsAnimalControlOfficer;
} //isAnimalControlOfficer

const bool CObject::isBossTurret() const 
{
    return m_bIsBossTurret;
} //isAnimalControlOfficer

const bool CObject::isGhost() const 
{
    return m_bIsGhost;
} //isAnimalControlOfficer