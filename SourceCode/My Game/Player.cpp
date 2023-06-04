/// \file Player.cpp
/// \brief Code for the player object class CPlayer.

#include "Player.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

/// Create and initialize an player object given its initial position.
/// \param p Initial position of player.

CPlayer::CPlayer(const Vector2& p) : CObject(eSprite::Player, p)
{
    startingPosition = m_vPos;
    m_bIsTarget = true;
    m_bStatic = false;
    m_bIsPlayer = true;
} //constructor

/// Move and rotate in response to device input. The amount of motion and
/// rotation speed is proportional to the frame time.

void CPlayer::move() {

    float stamTime = m_pTimer->GetTime();   //get new time for sprint
    float focusTime = m_pTimer->GetTime();  //get new time for focus
    float comboTime = m_pTimer->GetTime();  //get new time for combo
    float reloadTime = m_pTimer->GetTime(); //get new time for reload

    // SPRINTING CHECKS
    //checks if the player is running and runs out of stamina
    if (m_bIsDashing && (m_nStamina > 0))
    {
        //m_pPlayer->SetSpeed(200.0f + m_fMovementSpeedModifier);     //set sprint to 200 + the player's movement mod
        m_pPlayer->SetSpeed(120.0f);     //set sprint to 200 + the player's movement mod
    }
    else if (m_bIsDashing && (m_nStamina == 0)) //stop running if out of stamina
    {
        m_pPlayer->SetSpeed(0.0f);
    }

    //Has the player been running for more than 0.5 seconds, decrease stamina by 1 point
    if ((stamTime - m_fTimeLastDash >= 0.5f) && m_bIsDashing && (m_nStamina > 0))
    {
        m_nStamina--;
        m_fTimeLastDash = stamTime;
    }

    //Regenerate Stamina 1 point every second if the player isn't dashing
    if ((stamTime - m_fTimeLastDash >= 1.0f) && !m_bIsDashing)
    {
        if (m_nStamina < m_nMaxStamina)
        {
            m_nStamina++;
        }
        m_fTimeLastDash = stamTime;
    }

    // FOCUSING CHECKS
    //checks if the player is focusing and runs out of focus
    if (m_bIsFocusing && (m_nFocus <= 0))
    {
        m_bIsFocusing = false;
        m_pAudio->play(eSound::SlowMoEnd); //explosion
    }

    //Has the player been focusing for more than 0.5 seconds, decrease stamina by 1 point
    if ((focusTime - m_fTimeLastFocus >= 0.5f) && m_bIsFocusing && (m_nFocus > 0))
    {
        m_nFocus--;
        m_fTimeLastFocus = focusTime;
    }

    //Regenerate Focus 1 point every 2 seconds if the player isn't focusing
    if ((focusTime - m_fTimeLastFocus >= 2.0f) && !m_bIsFocusing)
    {
        if (m_nFocus < m_nMaxFocus)
        {
            m_nFocus++;
        }
        m_fTimeLastFocus = focusTime;
    }

    // COMBO CHECKS
    //decrease combo by 1 every 5 seconds after last confirmed hit
    if (comboTime - m_fTimeLastHit > 5.0f)
    {
        if (m_nCombo > 0)
        {
            m_nCombo--;
        }
        UpdatePlayerDamage();   //update player's damage so it display's properly in the text display
        m_fTimeLastHit = comboTime;
    }

    // RELOADING CHECKS
    //check if player is reloading a magazine based weapon and reload in an according amount of time.
    if (m_bIsReloading == true)
    {
        //change reload speed value based on currently selected weapon
        switch (m_nWeaponSelector)
        {
            //REVOLVER RELOAD
            case 0:                 
                if (reloadTime - m_fTimeLastReload > 0.75f) //this is so the player cant fire right after spamming a reload
                {
                    m_bIsReloading = false; // player should no longer be reloading
                }
                break;

            //PISTOL RELOAD
            case 1:         
                if (reloadTime - m_fTimeLastReload > m_fPistolReloadTime)  //reload the pistol after its reload speed's amount of time
                {
                    m_nPistolMag = m_nPistolFullMag;    //refill the magazine
                    m_bIsReloading = false; // player should no longer be reloading
                }
                break;
            //SHOTGUN RELOAD
            case 2: 
                if (reloadTime - m_fTimeLastReload > m_fShotgunReloadTime)  //reload the Shotgun after its reload speed's amount of time
                {
                    m_nShotgunMag = m_nShotgunFullMag;    //refill the magazine
                    m_bIsReloading = false; // player should no longer be reloading
                }
                break; // put the different weapon's reload speed values here
            //______ RELOAD
            case 3: break;
            //______ RELOAD
            case 4: break;
            default: m_bIsReloading = false; break;
        }
    }

    const float t = m_pTimer->GetFrameTime(); //time
    const Vector2 view = GetViewVector(); //view vector
    m_vPos += m_fSpeed * t * view; //move forwards
    m_fRoll += m_fRotSpeed * t; //rotate
    NormalizeAngle(m_fRoll); //normalize to [-pi, pi] for accuracy

    const Vector2 up = Vector2(-1, 0);
    const Vector2 right = Vector2(0, 1); // these 2 can be initialized elsewhere for cleaner code*****
    Vector2 vectorDir = Vector2(0, 0);
    //strafe

    const Vector2 norm = VectorNormalCC(view); //normal to view vector

    if (m_fMovementSpeed >= 170.0f)
    {
        m_fMovementSpeed = 170.0f;  //player's speed cannot be faster than this
    }

    float delta = (m_fMovementSpeed) * t; //change in position for strafing (removed const so dash can modify)

    if (m_bStrafeRight)//strafe right //Most of this is very dirty code*****
    {
        vectorDir -= right;
    }
    if (m_bStrafeLeft)//strafe left
    {
        vectorDir += right;
    }
    if (m_bStrafeBackward)//strafe backward
    {
        vectorDir -= up;
    }
    if (m_bStrafeForward)//strafe forward
    {
        vectorDir += up;
    }
    //setting strafes

    vectorDir = VectorNormalCC(vectorDir);
    m_vPos -= delta * vectorDir;

    m_bStrafeLeft = m_bStrafeRight = m_bStrafeBackward = m_bStrafeForward = false; //reset strafe flags
} //move

/// Response to collision. If the object being collided with is a bullet, then
/// play a sound, otherwise call `CObject::CollisionResponse` for the default
/// collision response.
/// \param norm Collision normal.
/// \param d Overlap distance.
/// \param pObj Pointer to object being collided with (defaults to `nullptr`,
/// which means collision with a wall).

void CPlayer::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
    if (m_bDead)return; //already dead, bail out 

    if (pObj && pObj->isAnimalControlOfficer() && !m_bGodMode)
    {
        m_pAudio->play(eSound::Boom); //explosion
        m_bDead = true; //flag for deletion from object list
        DeathFX(); //particle effects
        m_pPlayer = nullptr; //clear common player pointer
    }

    if (pObj && pObj->isGhost())
    {
        float currTime = m_pTimer->GetTime();

        if (currTime - m_fTimeLastGhostHit >= 1.0f)
        {
            m_fTimeLastGhostHit = currTime;

            if (m_bGodMode) //god mode, does no damage
                m_pAudio->play(eSound::Grunt);

            else //not in godmode
            {
                //Decrease combo
                if (m_nCombo > 0)   //don't even check this for if combo is 0
                {
                    for (int i = 0; i < 5; i++)    //increment at most 5 times
                    {
                        if (m_nCombo > 0)   //if combo isn't 0
                            m_nCombo -= 1;
                    }
                    UpdatePlayerDamage(); // update the player's damage after taking combo damage
                }

                //damage is different depending on difficulty
                int damageTicks = 0;

                if (m_pObjectManager->m_nDifficultyModifier >= 8)
                {
                    damageTicks = 10;
                }
                else if (m_pObjectManager->m_nDifficultyModifier >= 6)
                {
                    damageTicks = 8;
                }
                else if (m_pObjectManager->m_nDifficultyModifier >= 4)
                {
                    damageTicks = 6;
                }
                else if (m_pObjectManager->m_nDifficultyModifier >= 2)
                {
                    damageTicks = 4;
                }
                else
                {
                    damageTicks = 2;
                }
                for (int i = 0; i < damageTicks; i++)
                {
                    if (m_nHealth > 0)
                        m_nHealth--;
                }

                //dead?
                if (m_nHealth <= 0) //health decrements to zero means death 
                {
                    m_pAudio->play(eSound::Boom); //explosion
                    m_bDead = true; //flag for deletion from object list
                    DeathFX(); //particle effects
                    m_pPlayer = nullptr; //clear common player pointer
                    //Display damage text
                } //if
                else        //didn't die
                {
                    m_pAudio->play(eSound::Grunt); //impact sound
                }
            }
        }
    }

    if (pObj && pObj->isAnt()) //collision with ant
    {
        if (m_bGodMode) //god mode, does no damage
            m_pAudio->play(eSound::Grunt);

        else //not in godmode
        {
            //Decrease combo
            if (m_nCombo > 0)   //don't even check this for if combo is 0
            {
                for (int i = 0; i < 10; i++)    //increment at most 10 times
                {
                    if (m_nCombo > 0)   //if combo isn't 0
                        m_nCombo -= 1;
                }
                UpdatePlayerDamage(); // update the player's damage after taking combo damage
            }

            //damage is different depending on difficulty
            int damageTicks = 0;

            if (m_pObjectManager->m_nDifficultyModifier >= 8)
            {
                damageTicks = 11;
            }
            else if (m_pObjectManager->m_nDifficultyModifier >= 6)
            {
                damageTicks = 9;
            }
            else if (m_pObjectManager->m_nDifficultyModifier >= 4)
            {
                damageTicks = 7;
            }
            else if (m_pObjectManager->m_nDifficultyModifier >= 2)
            {
                damageTicks = 5;
            }
            else
            {
                damageTicks = 3;
            }
            for (int i = 0; i < damageTicks; i++)
            {
                if (m_nHealth > 0)
                    m_nHealth--;
            }

            //dead?
            if (m_nHealth <= 0) //health decrements to zero means death 
            {
                m_pAudio->play(eSound::Boom); //explosion
                m_bDead = true; //flag for deletion from object list
                DeathFX(); //particle effects
                m_pPlayer = nullptr; //clear common player pointer
                //Display damage text
            } //if
            else        //didn't die
            {
                m_pAudio->play(eSound::Grunt); //impact sound
            }
        }
    }

    if (pObj && pObj->isEnemyBullet())  //collision with an enemies bullet
    { 
        if (m_bGodMode) //god mode, does no damage
            m_pAudio->play(eSound::Grunt); //impact sound

        else //not in godmode
        {
            //Decrease combo
            if (m_nCombo > 0)   //don't even check this for if combo is 0
            {
                for (int i = 0; i < 3; i++)    //increment at most 3 times
                {
                    if (m_nCombo > 0)   //if combo isn't 0
                        m_nCombo -= 1;
                }
                UpdatePlayerDamage(); // update the player's damage after taking combo damage
            }

            //damage is different depending on difficulty
            int damageTicks = 0;

            if (m_pObjectManager->m_nDifficultyModifier >= 8)
            {
                damageTicks = 5;
            }
            else if (m_pObjectManager->m_nDifficultyModifier >= 6)
            {
                damageTicks = 4;
            }
            else if (m_pObjectManager->m_nDifficultyModifier >= 4)
            {
                damageTicks = 3;
            }
            else if (m_pObjectManager->m_nDifficultyModifier >= 2)
            {
                damageTicks = 2;
            }
            else
            {
                damageTicks = 1;
            }
            for (int i = 0; i < damageTicks; i++)
            {
                if (m_nHealth > 0)
                    m_nHealth--;
            }

            //dead?
            if (m_nHealth <= 0) //health decrements to zero means death 
            {
                m_pAudio->play(eSound::Boom); //explosion
                m_bDead = true; //flag for deletion from object list
                DeathFX(); //particle effects
                m_pPlayer = nullptr; //clear common player pointer
                //Display damage text
            } //if
            else        //didn't die
            {
                m_pAudio->play(eSound::Grunt); //impact sound
            }
        }

    } //if

    CObject::CollisionResponse(norm, d, pObj); //default collision response
} //CollisionResponse

/// Perform a particle effect to mark the death of the player.

void CPlayer::DeathFX() {
    LParticleDesc2D d; //particle descriptor
    d.m_vPos = m_vPos; //center particle at player center

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
    d.m_f4Tint = XMFLOAT4(Colors::OrangeRed);
    m_pParticleEngine->create(d);
} //DeathFX

//Player's damage modifier for use in combo and upgrades
void CPlayer::UpdatePlayerDamage()
{
    UINT weaponDamage = 0;

    //change damage value based on currently selected weapon
    switch (m_nWeaponSelector)
    {
        case 0: weaponDamage = m_nRevolverDamage; break;
        case 1: weaponDamage = m_nPistolDamage; break;   
        case 2: weaponDamage = m_nShotgunDamage; break;
        case 3: weaponDamage = m_nRevolverDamage; break;
        case 4: weaponDamage = m_nRevolverDamage; break;
    }

    //player's damage is modified base on their combo's range, as well as their number of upgrades
    if (m_nWeaponSelector != 2)     //Combo and damage modifiers are only for non shotguns
    {
        if (m_nCombo >= 30)                         //S: Savage
        {
            m_nPlayerDamage = weaponDamage + m_nDamageUpgrades + 5;
        }
        else if (m_nCombo < 30 && m_nCombo >= 20)   //A: Apocolyptic!
        {
            m_nPlayerDamage = weaponDamage + m_nDamageUpgrades + 4;
        }
        else if (m_nCombo < 20 && m_nCombo >= 15)   //B: Badass
        {
            m_nPlayerDamage = weaponDamage + m_nDamageUpgrades + 3;
        }
        else if (m_nCombo < 15 && m_nCombo >= 10)   //C: Crazy
        {
            m_nPlayerDamage = weaponDamage + m_nDamageUpgrades + 2;
        }
        else if (m_nCombo < 10 && m_nCombo >= 5)    //D: Dismal
        {
            m_nPlayerDamage = weaponDamage + m_nDamageUpgrades + 1;
        }
        else
        {
            m_nPlayerDamage = weaponDamage + m_nDamageUpgrades;
        }
    }
    else           //shotgun always does more damage, but damage modifiers instead change
    {
        m_nPlayerDamage = weaponDamage;

        if (m_nCombo >= 30)                         //S: Savage
        {
            m_nShotgunPellets = weaponDamage + m_nDamageUpgrades + 5;
        }
        else if (m_nCombo < 30 && m_nCombo >= 20)   //A: Apocolyptic!
        {
            m_nShotgunPellets = weaponDamage + m_nDamageUpgrades + 4;
        }
        else if (m_nCombo < 20 && m_nCombo >= 15)   //B: Badass
        {
            m_nShotgunPellets = weaponDamage + m_nDamageUpgrades + 3;
        }
        else if (m_nCombo < 15 && m_nCombo >= 10)   //C: Crazy
        {
            m_nShotgunPellets = weaponDamage + m_nDamageUpgrades + 2;
        }
        else if (m_nCombo < 10 && m_nCombo >= 5)    //D: Dismal
        {
            m_nShotgunPellets = weaponDamage + m_nDamageUpgrades + 1;
        }
        else
        {
            m_nShotgunPellets = weaponDamage + m_nDamageUpgrades;
        }
    }

}

/// Set the strafe left flag. This function will be called in response to
/// device inputs.

void CPlayer::StrafeLeft() {
    m_bStrafeLeft = true;
} //StrafeLeft

/// Set the strafe right flag. This function will be called in response to
/// device inputs.

void CPlayer::StrafeRight() {
    m_bStrafeRight = true;
} //StrafeRight

/// Set the strafe backward flag. This function will be called in response to
/// device inputs.

void CPlayer::StrafeBackward() {
    m_bStrafeBackward = true;
} //StrafeBackward

/// Set the strafe forward flag. This function will be called in response to
/// device inputs.

void CPlayer::StrafeForward() {
    m_bStrafeForward = true;
} //StrafeBackward

/// Set the object's speed, assuming that the object moves in the direction of
/// its view vector. This function will be called in response to device inputs.
/// \param speed Speed.

void CPlayer::SetSpeed(const float speed) {
    m_fSpeed = speed;
} //SetSpeed

/// Set the object's rotational speed in revolutions per second. This function
/// will be called in response to device inputs.
/// \param speed Rotational speed in RPS.

void CPlayer::SetRotSpeed(const float speed) {
    m_fRotSpeed = speed;
} //SetRotSpeed

/// Reader function for position.
/// \return Position.

const Vector2& CPlayer::GetPos() const {
    return m_vPos;
} //GetPos

void CPlayer::Aim(Vector2 RThumb)
{
    const float theta = atan2f(RThumb.y, RThumb.x); //orientation of that vector
    m_fRoll = theta;
}
