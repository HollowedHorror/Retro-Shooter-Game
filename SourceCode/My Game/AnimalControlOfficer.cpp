/// \file AnimalControlOfficer.cpp
/// \brief Code for the Animal Control Officer object class CAnimalControlOfficer.

#include "AnimalControlOfficer.h"
#include "ComponentIncludes.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "Helpers.h"
#include "Player.h"

/// Create and initialize an AnimalControlOfficer object given its initial position.
/// \param pos Initial position of AnimalControlOfficer.

CAnimalControlOfficer::CAnimalControlOfficer(const Vector2& pos) :
    CObject(eSprite::AnimalControlOfficer, pos)
{
    m_fRoll = -XM_PIDIV2; //facing up
    m_vVelocity = Vector2(0.0f, 64.0f); //going up
    m_fXScale = m_fYScale = 0.5f; //scale
    m_fRadius *= m_fXScale; //scale the bounding circle radius

    m_bIsTarget = true;
    m_bStatic = false;
    m_bIsAnimalControlOfficer = true;
} //constructor

/// Destructor.

CAnimalControlOfficer::~CAnimalControlOfficer() {
} //destructor

void CAnimalControlOfficer::move() {
    if (m_pPlayer != nullptr)
    {
        Follow(m_pPlayer->m_vPos);
        CObject::move(); //move like a default object
    }
} //move

void CAnimalControlOfficer::Follow(const Vector2& pos) {
    Vector2 v = pos - m_vPos; //vector from target to ghost

    if (abs(v.x) > 50 && abs(v.y) > 50)
    {
        v.x = v.x / 8;
        v.y = v.y / 8;
        m_vVelocity = v;
    }
    else if (abs(v.x) > 25 && abs(v.y) > 25)
    {
        v.x = v.x / 4;
        v.x = v.x / 4;
    }
    else
    {
        v.x = v.x;
        v.y = v.y;
        m_vVelocity = v;
    }
} //Follow

void CAnimalControlOfficer::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
    if (m_bDead)return; //already dead, bail out
} //CollisionResponse