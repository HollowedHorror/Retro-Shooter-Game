//PowerUp.cpp

#include "PowerUp.h"
#include "ComponentIncludes.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "Helpers.h"
#include "Player.h"

//Constructor for powerup
CPowerUp::CPowerUp(eSprite t, const Vector2& p) : CObject(t, p)
{
	m_bIsPowerUp = true; //is a powerup
	m_bStatic = true; //powerups are static
	switch (t)
	{
		case eSprite::Health:			m_bIsHealth = true;				break;
		case eSprite::HealthUp:			m_bIsHealthUp = true;			break;
		case eSprite::StaminaUp:		m_bIsStaminaUp = true;			break;
		case eSprite::FocusUp:			m_bIsFocusUp = true;			break;
		case eSprite::MovementSpeedUp:	m_bIsMovementSpeedUp = true;	break;
		case eSprite::DamageUp:			m_bIsDamageUp = true;			break;
		default: break;
	}
} //constructor/

// 
void CPowerUp::CollisionResponse(const Vector2& norm, float d, CObject* pObj) 
{
	if (m_pPlayer != nullptr) //crash safety
	{
		if (pObj && pObj->isPlayer())
		{
			if (m_bIsHealth)	//item is health pickup: increase player's current health
			{
				m_pAudio->play(eSound::Bend1);
				for (int i = 0; i < 3; i++)	//increase the player's health by 1, a number of times, check not to go over max
				{
					if (m_pPlayer->m_nHealth < m_pPlayer->m_nMaxHealth)
					{
						m_pPlayer->m_nHealth++;
					}
				}
			}
			if (m_bIsHealthUp)	//item is health up pickup: increase player's max health by 1 for the rest of the run
			{
				m_pAudio->play(eSound::PowerChord1);
				m_pPlayer->m_nMaxHealth++;	//increase max health by 1
				if (m_pPlayer->m_nHealth < m_pPlayer->m_nMaxHealth)	//add the health the player gained
				{
					m_pPlayer->m_nHealth++;
				}
			}
			if (m_bIsStaminaUp)	//item is stamina up pickup: increase player's max stamina by 1 for the rest of the run
			{
				m_pAudio->play(eSound::PowerChord2);
				m_pPlayer->m_nMaxStamina++;
			}
			if (m_bIsFocusUp)	//item is focus up pickup: increase player's max focus by 1 for the rest of the run
			{
				m_pAudio->play(eSound::PowerChord3);
				m_pPlayer->m_nMaxFocus++;
			}
			if (m_bIsMovementSpeedUp)	//item is movement speed up pickup: increase player's movement speed by _______ for the rest of the run
			{
				m_pAudio->play(eSound::Slide1);
				m_pPlayer->m_fMovementSpeedModifier = (15.0f / m_pPlayer->m_fMovementSpeed) * 100;	//parabolically increase movement speed, as to not get so fast you break out of the map.
				m_pPlayer->m_fMovementSpeed += m_pPlayer->m_fMovementSpeedModifier;
			}
			if (m_bIsDamageUp)	//item is damage up pickup: increase player's base damage by 1 for the rest of the run
			{
				m_pAudio->play(eSound::Chug2);
				m_pPlayer->m_nDamageUpgrades++;
			}
			m_bDead = true; //flag for deletion from object list
		}
	}
} //CollisionResponse