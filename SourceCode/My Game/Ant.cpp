/// \file Ant.cpp
/// \brief Code for the ant object class CAnt.

#include "Ant.h"
#include "ComponentIncludes.h"
#include "ObjectManager.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "Helpers.h"
#include "Player.h"

/// Create and initialize an ant object given its initial position.
/// \param pos Initial position of ant.

CAnt::CAnt(const Vector2& pos):
  CObject(eSprite::Ant, pos), 
  m_bPreferPosRot(m_pRandom->randf() < 0.5f)
{
  m_fRoll = -XM_PIDIV2; //facing up
  m_vVelocity = Vector2(0.0f, 64.0f); //going up
  m_fXScale = m_fYScale = 0.5f; //scale
  m_fRadius *= m_fXScale; //scale the bounding circle radius

  m_pFrameEvent = new LEventTimer(0.1f);
  m_pStrayEvent = new LEventTimer(5.0f, 2.0f);

  m_bIsTarget = true;
  m_bStatic = false;
  m_bIsAnt = true;
} //constructor

/// Destructor.

CAnt::~CAnt(){
  delete m_pFrameEvent;
  delete m_pStrayEvent;
} //destructor

/// Move and advance current frame number. Also stray randomly from current path.

void CAnt::move(){ 
  CObject::move(); //move like a default object
  StrayFromPath(); //stray randomly left or right
  UpdateFramenumber(); //choose current frame
} //move

/// Adjust direction randomly at random intervals.

void CAnt::StrayFromPath(){  
  const float t = m_pTimer->GetTime(); //current time
  
  if(m_pStrayEvent && m_pStrayEvent->Triggered()){ //enough time has passed
    const float delta = (m_bStrayParity? -1.0f: 1.0f)*0.1f; //angle delta

    m_vVelocity = RotateVector(m_vVelocity, delta); //change direction by delta
    m_fRoll += delta; //rotate to face that direction

    m_bStrayParity = m_pRandom->randf() < 0.5f; //next stray is randomly left or right 
  } //if
} //StrayFromPath

/// Update the frame number in the animation sequence.

void CAnt::UpdateFramenumber(){
  const size_t n = m_pRenderer->GetNumFrames(m_nSpriteIndex); //number of frames

  if(n > 1 && m_pFrameEvent && m_pFrameEvent->Triggered()){
    m_pFrameEvent->SetDelay(100.0f/(1500.0f + fabsf(m_fSpeed)));
    m_nCurrentFrame = (m_nCurrentFrame + 1)%n; 
  } //if
} //UpdateFramenumber

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

void CAnt::CollisionResponse(const Vector2& norm, float d, CObject* pObj){
  if (m_bDead)return; //already dead, bail out
  const float t = m_pTimer->GetFrameTime(); //frame time

  //start rotating if hit from behind

  if( m_vVelocity.Dot(norm) < 0 && this > pObj){ 
    const float delta = 4.0f*(m_bPreferPosRot? 1: -1)*t; //rotation delta
    m_vVelocity = RotateVector(m_vVelocity, delta); //rotate velocity
    m_fRoll += delta; //face the correct direction
  } //else

  //jump if hit by a bullet

  if (pObj && pObj->isPlayer())
  {
      //Don't spawn powerup if hit by ant

      m_pAudio->play(eSound::Boom); //explosion
      m_bDead = true; //Flags for deletion from object list
      DeathFX(); //particle effects
  }

  if(pObj && pObj->isPlayerBullet())
  {
      if (m_pPlayer != nullptr) //crash safety
      {
          m_pPlayer->m_fTimeLastHit = m_pTimer->GetTime();   //get time when player hit, this will be used in Player.cpp to lower combo 2 seconds after this
          m_pPlayer->m_nCombo++; //player hit an ant, increase combo
      }
      //spawn powerups
      srand(m_pTimer->GetTime());     //get random seed
      int randNum = rand() % 20 + 1;   //generate random number between 1 and the total types of powerups

      //create random powerup spawned on the location of the enemy
      switch (randNum)
      {
          //Switch cases don't let you select a range of values, but this naive approach works fine (other method would just use a bunch of if statements anyway)
          //Health +5
          case 1: m_pObjectManager->create(eSprite::Health, m_vPos);  break;
          case 2: m_pObjectManager->create(eSprite::Health, m_vPos);  break;
          //Max Health +1
          case 3: m_pObjectManager->create(eSprite::HealthUp, m_vPos);  break;
          case 4: m_pObjectManager->create(eSprite::HealthUp, m_vPos);  break;
          //Max Stamina +1
          case 5: m_pObjectManager->create(eSprite::StaminaUp, m_vPos);  break;
          //Max Focus +1
          case 6: m_pObjectManager->create(eSprite::FocusUp, m_vPos);  break;
          //Movement Speed Up +10.0f
          case 7: m_pObjectManager->create(eSprite::MovementSpeedUp, m_vPos);  break;
          case 8: m_pObjectManager->create(eSprite::MovementSpeedUp, m_vPos);  break;
          //Ants have a large chance to spawn nothing on death
          case 9:    break;
          case 10:   break;
          case 11:   break;
          case 12:   break;
          case 13:   break;
          case 14:   break;
          case 15:   break;
          case 16:   break;
          case 17:   break;
          case 18:   break;
          case 19:   break;
          case 20:   break;
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
      m_bDead = true; //Flags for deletion from object list
      DeathFX(); //particle effects
  } //if

  CObject::CollisionResponse(norm, d, pObj); //default collision response
} //CollisionResponse

void CAnt::DeathFX()
{
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
