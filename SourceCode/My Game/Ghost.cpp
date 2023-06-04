/// \file Ant.cpp
/// \brief Code for the ant object class CGhost.

#include "Ghost.h"
#include "ComponentIncludes.h"
#include "ObjectManager.h"
#include "TileManager.h"
#include "Player.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

/// Create and initialize an ant object given its initial position.
/// \param pos Initial position of ant.

CGhost::CGhost(const Vector2& pos) :
    CObject(eSprite::Ghost, pos)
{
    m_fRoll = -XM_PIDIV2; //facing up
    m_vVelocity = Vector2(0.0f, 20.0f); //going up
    m_fXScale = m_fYScale = 0.5f; //scale

    m_bIsTarget = true;
    m_bStatic = false;
    m_bIsGhost = true;
} //constructor

/// Destructor.

CGhost::~CGhost() {

} //destructor

void CGhost::move() 
{
    if (m_pPlayer != nullptr) //crash safety
    {
        if (!stopMoving)
        {
            Follow(m_pPlayer->m_vPos);
            CObject::move(); //move like a default object
        }
        else
        {
            stopMoving = false;
        }
    }
} //move

/// Response to collision. If the ant is facing the object that is colliding
/// with, then it rotates in its preferred direction and then calls 
/// CObject::CollisionResponse. Note that the only reason that colliding
/// ants won't be facing each other is in the case of multiple ants colliding
/// and pushing one of them in some direction other than the one in which
/// it is facing. Howerver, faster objects such as the player objact can
/// collide from behind.
/// \param norm Collision normal.
/// \param d Overlap distance.
/// \param pObj Pointer to object being collided with (defaults to nullptr,
/// which means collision with a wall).

void CGhost::Follow(const Vector2& pos) {
    Vector2 v = pos - m_vPos; //vector from target to ghost

    if (abs(v.x) > 50 && abs(v.y) > 50)
    {
        v.x = v.x / 4;
        v.y = v.y / 4;
        m_vVelocity = v;
    }
    else
    {
        v.x = v.x * 3;
        v.y = v.y * 3;
        m_vVelocity = v;
    }
} //Follow

void CGhost::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
    if (m_bDead)return; //already dead, bail out

    if (pObj && pObj->isPlayerBullet()) //enemy bullets dont kill ghosts
    {
        if (bulletSkip == 0)
        {
            m_pObjectManager->numOfGhosts--;
            m_bDead = true; //Flags for deletion from object list
            DeathFX(); //particle effects
        }
        else
        {
            bulletSkip--;
        }
    }

    if (pObj && pObj->isPlayer())
    {
        stopMoving = true;  //Flag to stop moving when in contact with player
    }
} //CollisionResponse

void CGhost::DeathFX()
{

} //DeathFX
