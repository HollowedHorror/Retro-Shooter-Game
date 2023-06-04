/// \file MGTurret.cpp
/// \brief Code for the MGTurret object class CMGTurret.

#include "MGTurret.h"
#include "ComponentIncludes.h"
#include "ObjectManager.h"
#include "TileManager.h"
#include "Player.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

/// Create and initialize a MGTurret object given its position.
/// \param p Position of MGTurret.

CMGTurret::CMGTurret(const Vector2& p) : CObject(eSprite::MGTurret, p)
{
    m_bIsTurret = true; //MGTurret is a MGTurret, used for enemy bullets to not collide with MGTurrets
    m_bStatic = true; //MGTurrets are static
} //constructor

/// Rotate the MGTurret and fire the gun at at the closest available target if
/// there is one, and rotate the MGTurret at a constant speed otherwise.

void CMGTurret::move()
{

    if (m_pPlayer) { //safety
        const float r = ((CMGTurret*)m_pPlayer)->m_fRadius; //player radius

        if (m_pTileManager->Visible(m_vPos, m_pPlayer->m_vPos, r)) //player visible
            RotateTowards(m_pPlayer->m_vPos);
        //else m_fRotSpeed = 0.0f; //no target visible, so stop
        else
        {
            RandomScan();
        }

    } //if

    m_fRoll += 0.2f * m_fRotSpeed * XM_2PI * m_pTimer->GetFrameTime(); //rotate
    NormalizeAngle(m_fRoll); //normalize to [-pi, pi] for accuracy
} //move

/// Rotate the MGTurret towards a point and file the gun if it is facing
/// sufficiently close to it.
/// \param pos Target point.

void CMGTurret::RotateTowards(const Vector2& pos) {
    const Vector2 v = pos - m_vPos; //vector from target to MGTurret
    const float theta = atan2f(v.y, v.x); //orientation of that vector
    float diff = m_fRoll - theta; //difference with MGTurret orientation
    NormalizeAngle(diff); //normalize to [-pi, pi]

    //set rotation speed from diff

    const float fAngleDelta = 0.05f; //allowable angle discrepancy
    const float fTrackingSpeed = 1.7f; //rotation speed when tracking

    if (diff > fAngleDelta)m_fRotSpeed = -fTrackingSpeed; //clockwise
    else if (diff < -fAngleDelta)m_fRotSpeed = fTrackingSpeed; //counterclockwise
    else m_fRotSpeed = 0; //stop rotating

    //fire gun if pointing approximately towards target

    float currTime = m_pTimer->GetTime();

    if (fabsf(diff) < fAngleDelta && currTime - m_fLastTimeFire >= 0.35f)
    {
        m_fLastTimeFire = currTime;
        m_pObjectManager->FireGun(this, eSprite::Bullet2);
    }
} //RotateTowards

void CMGTurret::RandomScan()
{
    float time = m_pTimer->GetTime();//All to "scan" every 3 seconds
    if (time - timeElapsed > 3.0f && scan) //update scan manuever every 3 seconds
    {
        random = rand() % 3;
        scan = false;
        timeElapsed = time;
    }
    else if (timeElapsed == 0.0) //Beginning of level scan
    {
        random = rand() % 3;
        timeElapsed = 0.1;
    }
    else scan = true;

    if (random == 0) //Turn Left, Right, or none w/ a random speed between high&low
    {
        m_fRotSpeed = 0.0;
    }
    if (random == 1)
    {
        m_fRotSpeed = -high + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
    }
    if (random == 2)
    {
        m_fRotSpeed = high + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));;
    }
}

/// Response to collision. 
/// \param norm Collision normal.
/// \param d Overlap distance.
/// \param pObj Pointer to object being collided with (defaults to `nullptr`,
/// which means collision with a wall).

void CMGTurret::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
    if (m_bDead)return; //already dead, bail out

    if (pObj && pObj->isBullet() && pObj->isPlayerBullet()) //collision with a player's bullet
    {
        if (m_pPlayer != nullptr) //crash safety
        {
            m_pPlayer->m_fTimeLastHit = m_pTimer->GetTime();   //get time when player hit, this will be used in Player.cpp to lower combo 2 seconds after this
            m_pPlayer->m_nCombo++; //player hit a MGTurret, increase combo
        }

        if (m_pPlayer != nullptr) //crash safety
        {
            for (int i = 0; i < m_pPlayer->m_nPlayerDamage; i++)    //decrease health by player's damage modifier
            {
                if (m_nHealth > 0)
                    m_nHealth--;
            }
        }

        //m_nHealth -= m_pPlayer->m_nPlayerDamage;  //decrease health by player's damage modifier   THIS BREAKS THE MGTurret BECAUSE IT CAUSES ITS HEALTH TO GO NEGATIVE

        if (m_nHealth == 0)   //health decrements to zero means death
        {
            //spawn powerups
            srand(m_pTimer->GetTime());     //get random seed
            int randNum = rand() % 20 + 1;   //generate random number between 1 and the total types of powerups

            //create random powerup spawned on the location of the enemy
            switch (randNum)
            {
                //Switch cases don't let you select a range of values, but this naive approach works fine (other method would just use a bunch of if statements anyway)
                //Current Health +
            case 1: m_pObjectManager->create(eSprite::Health, m_vPos);  break;
            case 2: m_pObjectManager->create(eSprite::Health, m_vPos);  break;
            case 3: m_pObjectManager->create(eSprite::Health, m_vPos);  break;
                //Max Health +1
            case 4: m_pObjectManager->create(eSprite::HealthUp, m_vPos);  break;
            case 5: m_pObjectManager->create(eSprite::HealthUp, m_vPos);  break;
            case 6: m_pObjectManager->create(eSprite::HealthUp, m_vPos);  break;
                //Max Stamina +1
            case 7: m_pObjectManager->create(eSprite::StaminaUp, m_vPos);  break;
            case 8: m_pObjectManager->create(eSprite::StaminaUp, m_vPos);  break;

                //Max Focus +1
            case 9: m_pObjectManager->create(eSprite::FocusUp, m_vPos);  break;
            case 10: m_pObjectManager->create(eSprite::FocusUp, m_vPos);  break;
            case 11: m_pObjectManager->create(eSprite::FocusUp, m_vPos);  break;
                //Movement Speed Up +10.0f
            case 12: m_pObjectManager->create(eSprite::MovementSpeedUp, m_vPos);  break;
            case 13: m_pObjectManager->create(eSprite::MovementSpeedUp, m_vPos);  break;
            case 14: m_pObjectManager->create(eSprite::MovementSpeedUp, m_vPos);  break;
                //Base Damage for all guns +1
            case 15: m_pObjectManager->create(eSprite::DamageUp, m_vPos);  break;
                //MGTurrets have a small change to spawn nothing on death
            case 16: break;
            case 17: break;
            case 18: break;
            case 19: break;
            case 20: break;
            }

            //Get random number to spawn ghost 50% of the time.
            srand(m_pTimer->GetTime());
            int spawnGhost = rand() % 2 + 1;

            //Spawn ghost is we have not reached maximum ghosts and spawnGhost = 1.
            if ((m_pObjectManager->numOfGhosts < m_pObjectManager->maxGhosts) && (spawnGhost == 1))
            {
                m_pObjectManager->create(eSprite::Ghost, m_vPos);
                m_pObjectManager->numOfGhosts++;
            }

            //initiate death
            m_pAudio->play(eSound::Boom); //explosion
            m_bDead = true; //flag for deletion from object list
            DeathFX(); //particle effects
        } //if

        else        //not a death blow
        {
            m_pAudio->play(eSound::Clang); //impact sound
            const float f = 0.5f + 0.5f * (float)m_nHealth / m_nMaxHealth; //health fraction
            m_f4Tint = XMFLOAT4(1.0f, f, f, 0); //redden the sprite to indicate damage
        } //else
    } //if
} //CollisionResponse

/// Perform a particle effect to mark the death of the MGTurret.

void CMGTurret::DeathFX() {
    LParticleDesc2D d; //particle descriptor
    d.m_vPos = m_vPos; //center particle at MGTurret center

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