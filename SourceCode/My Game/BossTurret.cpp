/// \file Turret.cpp
/// \brief Code for the turret object class CBossTurret.

#include "BossTurret.h"
#include "ComponentIncludes.h"
#include "ObjectManager.h"
#include "TileManager.h"
#include "Player.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

/// Create and initialize a turret object given its position.
/// \param p Position of turret.

CBossTurret::CBossTurret(const Vector2& p) : CObject(eSprite::BossTurret, p) {
    m_bStatic = true; //turrets are static
    m_fXScale = m_fYScale = 0.5f;
    m_bIsBossTurret = true;
} //constructor

/// Rotate the turret and fire the gun at at the closest available target if
/// there is one, and rotate the turret at a constant speed otherwise.

void CBossTurret::move()
{
    if (stage == 1)
    {
        float currTime = m_pTimer->GetTime() - 1.0f;

        if (m_pPlayer) { //safety
            const float r = ((CBossTurret*)m_pPlayer)->m_fRadius; //player radius

            if (m_pTileManager->Visible(m_vPos, m_pPlayer->m_vPos, r)) //player visible
            {
                RotateTowards(m_pPlayer->m_vPos);

                if (currTime - m_fTimeLastFire >= 0.2f)
                {
                    m_fTimeLastFire = currTime;
                    m_pObjectManager->FireGun(this, eSprite::Bullet2);
                }
            }
            else m_fRotSpeed = 0.0f; //no target visible, so stop
        } //if

        m_fRoll += 0.2f * m_fRotSpeed * XM_2PI * m_pTimer->GetFrameTime(); //rotate
        NormalizeAngle(m_fRoll); //normalize to [-pi, pi] for accuracy
    }
    else if (stage == 2)
    {
        float currTime = m_pTimer->GetTime();

        m_fRotSpeed = 2.0f;
        m_fRoll += 0.2f * m_fRotSpeed * XM_2PI * m_pTimer->GetFrameTime(); //rotate
        NormalizeAngle(m_fRoll); //normalize to [-pi, pi] for accuracy

        if (currTime - m_fTimeLastFire >= 0.1f)
        {
            m_fTimeLastFire = currTime;
            m_pObjectManager->FireGun(this, eSprite::Bullet2);
        }
    }
    else if (stage == 3)
    {
        float currTimeMoveSet = m_pTimer->GetTime();

        if (currTimeMoveSet - m_fTimeLastSwitch >= 5.0f)
        {
            m_fTimeLastSwitch = currTimeMoveSet;
            srand(m_pTimer->GetTime());
            movesetChoice = rand() % 2 + 1;
        }

        if (movesetChoice == 1)
        {
            float currTimeShoot = m_pTimer->GetTime() - 1.0f;

            if (m_pPlayer) { //safety
                const float r = ((CBossTurret*)m_pPlayer)->m_fRadius; //player radius

                if (m_pTileManager->Visible(m_vPos, m_pPlayer->m_vPos, r)) //player visible
                {
                    RotateTowards(m_pPlayer->m_vPos);

                    if (currTimeShoot - m_fTimeLastFire >= 0.2f)
                    {
                        m_fTimeLastFire = currTimeShoot;
                        m_pObjectManager->FireGun(this, eSprite::Bullet2);
                    }
                }
                else m_fRotSpeed = 0.0f; //no target visible, so stop
            } //if

            m_fRoll += 0.2f * m_fRotSpeed * XM_2PI * m_pTimer->GetFrameTime(); //rotate
            NormalizeAngle(m_fRoll); //normalize to [-pi, pi] for accuracy
        }
        else if (movesetChoice == 2)
        {
            float currTimeShoot = m_pTimer->GetTime();

            m_fRotSpeed = 2.0f;
            m_fRoll += 0.2f * m_fRotSpeed * XM_2PI * m_pTimer->GetFrameTime(); //rotate
            NormalizeAngle(m_fRoll); //normalize to [-pi, pi] for accuracy

            if (currTimeShoot - m_fTimeLastFire >= 0.1f)
            {
                m_fTimeLastFire = currTimeShoot;
                m_pObjectManager->FireGun(this, eSprite::Bullet2);
            }
        }
    }
} //move

/// Rotate the turret towards a point and file the gun if it is facing
/// sufficiently close to it.
/// \param pos Target point.

void CBossTurret::RotateTowards(const Vector2& pos) {
    const Vector2 v = pos - m_vPos; //vector from target to turret
    const float theta = atan2f(v.y, v.x); //orientation of that vector
    float diff = m_fRoll - theta; //difference with turret orientation
    NormalizeAngle(diff); //normalize to [-pi, pi]

    //set rotation speed from diff

    const float fAngleDelta = 0.05f; //allowable angle discrepancy
    const float fTrackingSpeed = 1.7f; //rotation speed when tracking

    if (diff > fAngleDelta)m_fRotSpeed = -fTrackingSpeed; //clockwise
    else if (diff < -fAngleDelta)m_fRotSpeed = fTrackingSpeed; //counterclockwise
    else m_fRotSpeed = 0; //stop rotating
} //RotateTowards

/// Response to collision. 
/// \param norm Collision normal.
/// \param d Overlap distance.
/// \param pObj Pointer to object being collided with (defaults to `nullptr`,
/// which means collision with a wall).

void CBossTurret::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
    if (m_bDead)return; //already dead, bail out 

    if (pObj && pObj->isBullet()) { //collision with bullet
        if (--m_nHealth == 0) { //health decrements to zero means death 
            m_pAudio->play(eSound::Bend3); //Hurray Boss DEAD!!
            m_pAudio->play(eSound::Boom); //explosion
            m_bDead = true; //flag for deletion from object list
            DeathFX(); //particle effects
        } //if

        else if(m_nHealth > 60) { //not a death blow and still on stage 1
            m_pAudio->play(eSound::Clang); //impact sound
            const float f = 0.5f + 0.5f * (float)m_nHealth / m_nMaxHealth; //health fraction
            m_f4Tint = XMFLOAT4(1.0f, f, f, 0); //redden the sprite to indicate damage
            stage = 1;
        } //else if

        else if (m_nHealth > 30) { //not a death blow and still on stage 2
            m_pAudio->play(eSound::Clang); //impact sound
            const float f = 0.5f + 0.5f * (float)m_nHealth / m_nMaxHealth; //health fraction
            m_f4Tint = XMFLOAT4(1.0f, f, f, 0); //redden the sprite to indicate damage
            stage = 2;
        } //else if

        else if (m_nHealth > 0) { //not a death blow and still on stage 3
            m_pAudio->play(eSound::Clang); //impact sound
            const float f = 0.5f + 0.5f * (float)m_nHealth / m_nMaxHealth; //health fraction
            m_f4Tint = XMFLOAT4(1.0f, f, f, 0); //redden the sprite to indicate damage
            stage = 3;
        }
    } //if
} //CollisionResponse

/// Perform a particle effect to mark the death of the turret.

void CBossTurret::DeathFX() {
    LParticleDesc2D d; //particle descriptor
    d.m_vPos = m_vPos; //center particle at turret center

    d.m_nSpriteIndex = (UINT)eSprite::Smoke;
    d.m_fLifeSpan = 2.0f;
    d.m_fMaxScale = 4.0f;
    d.m_fScaleInFrac = 0.5f;
    d.m_fFadeOutFrac = 0.8f;
    d.m_fScaleOutFrac = 0;
    m_pParticleEngine->create(d);

    d.m_nSpriteIndex = (UINT)eSprite::Spark;
    d.m_fLifeSpan = 0.5f;
    d.m_fMaxScale = 1.5f;
    d.m_fScaleInFrac = 0.4f;
    d.m_fScaleOutFrac = 0.3f;
    d.m_fFadeOutFrac = 0.5f;
    d.m_f4Tint = XMFLOAT4(Colors::Orange);
    m_pParticleEngine->create(d);
} //DeathFX