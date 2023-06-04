/// \file Game.cpp
/// \brief Code for the game class CGame.

#include "Game.h"

#include "GameDefines.h"
//#include "ExtRenderer.h"
#include "SpriteRenderer.h"
#include "ComponentIncludes.h"
#include "ParticleEngine.h"
#include "TileManager.h"

#include "shellapi.h"

#include "Player.h"     //to get access to player's health for display of 'healthbar'
#include "BarDisplay.h"

/// Delete the renderer, the object manager, and the tile manager. The renderer
/// needs to be deleted before this destructor runs so it will be done elsewhere.

CGame::~CGame()
{
    delete m_pParticleEngine;
    delete m_pObjectManager;
    delete m_pTileManager;
    delete m_pBarDisplay;
} //destructor

/// Initialize the renderer, the tile manager and the object manager, load 
/// images and sounds, and begin the game.

void CGame::Initialize()
{
    //m_pRenderer = new ExtRenderer(eSpriteMode::Batched2D);
    m_pRenderer = new LSpriteRenderer(eSpriteMode::Batched2D);
    m_pRenderer->Initialize(eSprite::Size);
    LoadImages(); //load images from xml file list

    m_pTileManager = new CTileManager((size_t)m_pRenderer->GetWidth(eSprite::Tile));
    m_pObjectManager = new CObjectManager; //set up the object manager 
    LoadSounds(); //load the sounds for this game

    m_pParticleEngine = new LParticleEngine2D(m_pRenderer);

    BeginGame();
} //Initialize

/// Load the specific images needed for this game. This is where `eSprite`
/// values from `GameDefines.h` get tied to the names of sprite tags in
/// `gamesettings.xml`. Those sprite tags contain the name of the corresponding
/// image file. If the image tag or the image file are missing, then the game
/// should abort from deeper in the Engine code leaving you with an error
/// message in a dialog box.

void CGame::LoadImages()
{
    m_pRenderer->BeginResourceUpload();

    m_pRenderer->Load(eSprite::Tile, "tile");
    m_pRenderer->Load(eSprite::Player, "player");
    m_pRenderer->Load(eSprite::Bullet, "bullet");
    m_pRenderer->Load(eSprite::Bullet2, "bullet2");
    m_pRenderer->Load(eSprite::Smoke, "smoke");
    m_pRenderer->Load(eSprite::Spark, "spark");
    m_pRenderer->Load(eSprite::Turret, "turret");
    m_pRenderer->Load(eSprite::MGTurret, "MGturret");
    m_pRenderer->Load(eSprite::Line, "greenline");
    m_pRenderer->Load(eSprite::Bar, "bar");

    //powerups
    m_pRenderer->Load(eSprite::Health, "health");
    m_pRenderer->Load(eSprite::HealthUp, "healthup");
    m_pRenderer->Load(eSprite::StaminaUp, "staminaup");
    m_pRenderer->Load(eSprite::FocusUp, "focusup");
    m_pRenderer->Load(eSprite::MovementSpeedUp, "movementspeedup");
    m_pRenderer->Load(eSprite::DamageUp, "damageup");

    m_pRenderer->Load(eSprite::Door, "door");
    m_pRenderer->Load(eSprite::BossTurret, "bossturret");
    m_pRenderer->Load(eSprite::AnimalControlOfficer, "animalcontrolofficer");
    m_pRenderer->Load(eSprite::Ghost, "ghost");
    m_pRenderer->Load(eSprite::AntSpriteSheet, "antwalk");
    m_pRenderer->Load(eSprite::Ant, "ant");

    m_pRenderer->EndResourceUpload();
} //LoadImages

/// Initialize the audio player and load game sounds.

void CGame::LoadSounds()
{
    m_pAudio->Initialize(eSound::Size);

    m_pAudio->Load(eSound::Grunt, "grunt");
    m_pAudio->Load(eSound::Clang, "clang");
    m_pAudio->Load(eSound::Gun, "gun");
    m_pAudio->Load(eSound::Ricochet, "ricochet");
    m_pAudio->Load(eSound::Start, "start");
    m_pAudio->Load(eSound::Boom, "boom");
    m_pAudio->Load(eSound::Ow, "ow");
    m_pAudio->Load(eSound::FeroLoop, "feroLoop");
    m_pAudio->Load(eSound::FeroLoop, "feroLoop");
    m_pAudio->Load(eSound::SlowMoStart, "slowMoStart");
    m_pAudio->Load(eSound::SlowMoEnd, "slowMoEnd");

    //Load Garrett's Sounds
    //Gun Sounds
    m_pAudio->Load(eSound::Pistolclick, "9MMclick");       //
    m_pAudio->Load(eSound::Pistolreload, "9MMreload");  //
    m_pAudio->Load(eSound::AKreload, "AKreload");   //
    m_pAudio->Load(eSound::AKrack, "AKrack");
    m_pAudio->Load(eSound::AKclick, "AKclick");   //
    m_pAudio->Load(eSound::bulletload, "bulletload");   //
    //Guitar Sounds
    m_pAudio->Load(eSound::Bend1, "Bend1"); //
    m_pAudio->Load(eSound::Bend2, "Bend2");
    m_pAudio->Load(eSound::Bend3, "Bend3"); //
    m_pAudio->Load(eSound::Chug1, "Chug1"); //
    m_pAudio->Load(eSound::Chug2, "Chug2"); //
    m_pAudio->Load(eSound::PowerChord1, "PowerChord1"); //
    m_pAudio->Load(eSound::PowerChord2, "PowerChord2"); //
    m_pAudio->Load(eSound::PowerChord3, "PowerChord3"); //
    m_pAudio->Load(eSound::PowerChord4, "PowerChord4");
    m_pAudio->Load(eSound::Slide1, "Slide1"); //
    m_pAudio->Load(eSound::Octave1, "Octave1");  //
    m_pAudio->Load(eSound::HappyChord1, "HappyChord1"); //
} //LoadSounds

/// Release all of the DirectX12 objects by deleting the renderer.

void CGame::Release() {
    delete m_pRenderer;
    m_pRenderer = nullptr; //for safety
    //m_pBarDisplay->Release();
} //Release

/// Ask the object manager to create a player object and turrets specified by
/// the tile manager.

void CGame::CreateObjects()
{
    std::vector<Vector2> turretpos; //vector of turret positions
    std::vector<Vector2> MGTurretPos; //vector of MG turret positions
    std::vector<Vector2> antpos; //vector of ant positions
    Vector2 playerpos; //player positions
    std::vector<Vector2> ghostpos; //vector of ghost positions
    std::vector<Vector2> bosspos; //boss positions

    m_pTileManager->GetObjects(turretpos, MGTurretPos, antpos, playerpos, ghostpos, bosspos); //get positions

    m_pPlayer = (CPlayer*)m_pObjectManager->create(eSprite::Player, playerpos);

    for (const Vector2& pos : turretpos)
        m_pObjectManager->create(eSprite::Turret, pos);

    for (const Vector2& pos : MGTurretPos)
        m_pObjectManager->create(eSprite::MGTurret, pos);

    for (const Vector2& pos : antpos)
        m_pObjectManager->create(eSprite::Ant, pos);

    for (const Vector2& pos : ghostpos)
        m_pObjectManager->create(eSprite::Ghost, pos);

    for (const Vector2& pos : bosspos)
        m_pObjectManager->create(eSprite::BossTurret, pos);

} //CreateObjects

/// Call this function to start a new game. This should be re-entrant so that
/// you can restart a new game without having to shut down and restart the
/// program. Clear the particle engine to get rid of any existing particles,
/// delete any old objects out of the object manager and create some new ones.

void CGame::BeginGame()
{
    m_pParticleEngine->clear(); //clear old particles

    if (m_pRandom->randf() < 0.5f)
    {
        if (m_pRandom->randf() < 0.5f)
        {
            switch (m_nNextLevel) {                                                 //HERE is where levels are loaded!!
            case 0: m_pTileManager->LoadMap("Media\\Maps\\map1a.txt"); break;
            case 1: m_pTileManager->LoadMap("Media\\Maps\\map2a.txt"); break;
            case 2: m_pTileManager->LoadMap("Media\\Maps\\map3a.txt"); break;
            case 3: m_pTileManager->LoadMap("Media\\Maps\\map4a.txt"); break;
            case 4: m_pTileManager->LoadMap("Media\\Maps\\map5a.txt"); break;
            case 5: m_pTileManager->LoadMap("Media\\Maps\\map6a.txt"); break;
            case 6: m_pTileManager->LoadMap("Media\\Maps\\map7a.txt"); break;
            case 7: m_pTileManager->LoadMap("Media\\Maps\\map8a.txt"); break;
            case 8: m_pTileManager->LoadMap("Media\\Maps\\map9a.txt"); break;
            case 9: m_pTileManager->LoadMap("Media\\Maps\\boss.txt"); break;
            } //switch
        }
        else
        {
            switch (m_nNextLevel) {                                                 //HERE is where levels are loaded!!
            case 0: m_pTileManager->LoadMap("Media\\Maps\\map1c.txt"); break;
            case 1: m_pTileManager->LoadMap("Media\\Maps\\map2c.txt"); break;
            case 2: m_pTileManager->LoadMap("Media\\Maps\\map3c.txt"); break;
            case 3: m_pTileManager->LoadMap("Media\\Maps\\map4c.txt"); break;
            case 4: m_pTileManager->LoadMap("Media\\Maps\\map5c.txt"); break;
            case 5: m_pTileManager->LoadMap("Media\\Maps\\map6c.txt"); break;
            case 6: m_pTileManager->LoadMap("Media\\Maps\\map7c.txt"); break;
            case 7: m_pTileManager->LoadMap("Media\\Maps\\map8c.txt"); break;
            case 8: m_pTileManager->LoadMap("Media\\Maps\\map9c.txt"); break;
            case 9: m_pTileManager->LoadMap("Media\\Maps\\boss.txt"); break;
            } //switch
        }
    }
    else
    {
        if (m_pRandom->randf() < 0.5f)
        {
            switch (m_nNextLevel) {                                                 //HERE is where levels are loaded!!
            case 0: m_pTileManager->LoadMap("Media\\Maps\\map1b.txt"); break;
            case 1: m_pTileManager->LoadMap("Media\\Maps\\map2b.txt"); break;
            case 2: m_pTileManager->LoadMap("Media\\Maps\\map3b.txt"); break;
            case 3: m_pTileManager->LoadMap("Media\\Maps\\map4b.txt"); break;
            case 4: m_pTileManager->LoadMap("Media\\Maps\\map5b.txt"); break;
            case 5: m_pTileManager->LoadMap("Media\\Maps\\map6b.txt"); break;
            case 6: m_pTileManager->LoadMap("Media\\Maps\\map7b.txt"); break;
            case 7: m_pTileManager->LoadMap("Media\\Maps\\map8b.txt"); break;
            case 8: m_pTileManager->LoadMap("Media\\Maps\\map9b.txt"); break;
            case 9: m_pTileManager->LoadMap("Media\\Maps\\boss.txt"); break;
            } //switch
        }
        else
        {
            switch (m_nNextLevel) {                                                 //HERE is where levels are loaded!!
            case 0: m_pTileManager->LoadMap("Media\\Maps\\map1d.txt"); break;
            case 1: m_pTileManager->LoadMap("Media\\Maps\\map2d.txt"); break;
            case 2: m_pTileManager->LoadMap("Media\\Maps\\map3d.txt"); break;
            case 3: m_pTileManager->LoadMap("Media\\Maps\\map4d.txt"); break;
            case 4: m_pTileManager->LoadMap("Media\\Maps\\map5d.txt"); break;
            case 5: m_pTileManager->LoadMap("Media\\Maps\\map6d.txt"); break;
            case 6: m_pTileManager->LoadMap("Media\\Maps\\map7d.txt"); break;
            case 7: m_pTileManager->LoadMap("Media\\Maps\\map8d.txt"); break;
            case 8: m_pTileManager->LoadMap("Media\\Maps\\map9d.txt"); break;
            case 9: m_pTileManager->LoadMap("Media\\Maps\\boss.txt"); break;
            } //switch
        }
    }

    //### Uncomment whichever level you want to test, and comment out the above switch case ###
    //m_pTileManager->LoadMap("Media\\Maps\\map1.txt");
    //m_pTileManager->LoadMap("Media\\Maps\\map2.txt");
    //m_pTileManager->LoadMap("Media\\Maps\\map3.txt");
    //m_pTileManager->LoadMap("Media\\Maps\\map4.txt");
    //m_pTileManager->LoadMap("Media\\Maps\\map5.txt");
    //m_pTileManager->LoadMap("Media\\Maps\\map6.txt");
    //m_pTileManager->LoadMap("Media\\Maps\\map7.txt");
    //m_pTileManager->LoadMap("Media\\Maps\\map8.txt");
    //m_pTileManager->LoadMap("Media\\Maps\\map9.txt");
    //m_pTileManager->LoadMap("Media\\Maps\\boss.txt");
    //m_pTileManager->LoadMap("Media\\Maps\\TEST.txt");
    //###                                                                                   ###

    m_pObjectManager->clear(); //clear old objects
    CreateObjects(); //create new objects (must be after map is loaded)

    //Update the player's stat values after the player object has been created
    m_pPlayer->m_nMaxHealth = m_nMaxHealthS;
    m_pPlayer->m_nMaxFocus = m_nMaxFocusS;
    m_pPlayer->m_nMaxStamina = m_nMaxStaminaS;
    m_pPlayer->m_fMovementSpeed = m_fMovementSpeedS;
    m_pPlayer->m_fMovementSpeedModifier = m_fMovementSpeedModifierS;
    m_pPlayer->m_nDamageUpgrades = m_nDamageUpgradesS;
    m_pPlayer->m_nHealth = m_nHealthS;
    m_pPlayer->m_nFocus = m_nFocusS;
    m_pPlayer->m_nStamina = m_nStaminaS;
    m_pPlayer->m_nWeaponSelector = m_nWeaponSelectorS;

    //m_pAudio->stop(); //stop all  currently playing sounds
    m_pAudio->play(eSound::Chug1); //play start-of-game sound
    m_eGameState = eGameState::Playing; //now playing
} //BeginGame

/// Poll the keyboard state and respond to the key presses that happened since
/// the last frame.

void CGame::KeyboardHandler()
{
    float first_push = 0, second_push = 0;     //time float for double pressing keys (used for dashing)

    m_pKeyboard->GetState(); //get current keyboard state

    if (m_pKeyboard->TriggerDown(VK_F1)) //help
        ShellExecute(0, 0, "https://larc.unt.edu/code/topdown/", 0, 0, SW_SHOW);

    if (m_pKeyboard->TriggerDown(VK_F2)) //toggle frame rate
        m_bDrawFrameRate = !m_bDrawFrameRate;

    if (m_pKeyboard->TriggerDown(VK_F3)) //toggle AABB drawing
        m_bDrawAABBs = !m_bDrawAABBs;

    if (m_pKeyboard->TriggerDown(VK_BACK)) //start game
        BeginGame();

    if (m_pPlayer) 
    { //safety

        if (m_pKeyboard->TriggerDown(VK_UP)) //move forwards
            m_pPlayer->SetSpeed(100.0f);

        if (m_pKeyboard->TriggerUp(VK_UP)) //stop
            m_pPlayer->SetSpeed(0.0f);

        //sprint: makes player move quickly towards where they are facing
        if (m_pKeyboard->TriggerDown(VK_LSHIFT)) 
        {
            if (m_pPlayer->m_nStamina > 0)     //if stamina is not empty
            {
                m_pPlayer->m_fTimeLastDash = m_pTimer->GetTime();
                m_pPlayer->m_bIsDashing = true;
                m_pPlayer->m_nStamina--;    //immediately lower stamina on press, to prevent sprint spamming
            }
            else
            {
                m_pPlayer->SetSpeed(0.0f);
            }
        }

        //stop sprinting
        if (m_pKeyboard->TriggerUp(VK_LSHIFT)) 
        {
            m_pPlayer->SetSpeed(0.0f);
            m_pPlayer->m_bIsDashing = false;
        }

        if (m_pKeyboard->TriggerDown(VK_RIGHT)) //rotate clockwise
            m_pPlayer->SetRotSpeed(-2.5f);

        if (m_pKeyboard->TriggerUp(VK_RIGHT)) //stop rotating clockwise
            m_pPlayer->SetRotSpeed(0.0f);

        if (m_pKeyboard->TriggerDown(VK_LEFT)) //rotate counterclockwise
            m_pPlayer->SetRotSpeed(2.5f);

        if (m_pKeyboard->TriggerUp(VK_LEFT)) //stop rotating counterclockwise
            m_pPlayer->SetRotSpeed(0.0f);

        if (m_pKeyboard->TriggerDown(VK_SPACE)) //fire gun
        {
            switch (m_pPlayer->m_nWeaponSelector)
            {
                case 0:
                    if ((m_pPlayer->m_nRevolverMag > 0) && (m_pPlayer->m_bIsReloading == false))  //if gun is not empty
                    {
                        m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet);  // fire the gun
                        m_pPlayer->m_nRevolverMag -= 1; // remove 1 bullet from the magazine
                    }
                    else
                    {
                        m_pAudio->play(eSound::Pistolclick);
                    }
                    break;
                case 1:  
                    if ((m_pPlayer->m_nPistolMag > 0) && (m_pPlayer->m_bIsReloading == false))  //if gun is not empty
                    {
                        m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet);  // fire the gun
                        m_pPlayer->m_nPistolMag -= 1; // remove 1 bullet from the magazine
                    }
                    else
                    {
                        m_pAudio->play(eSound::Pistolclick);
                    }
                    break;   //pistol is a magazine based reload, so set 'm_bIsReloading' to true
                case 2:  
                    if ((m_pPlayer->m_nShotgunMag > 0) && (m_pPlayer->m_bIsReloading == false))  //if gun is not empty
                    {
                        for (int i = 0; i < m_pPlayer->m_nShotgunPellets; i++)  //fire a bullet for whatever the damage modifier would be instead of actually increasing the damage
                        {
                            m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet);  // fire the gun
                        }

                        m_pPlayer->m_nShotgunMag -= 1; // remove 1 bullet from the magazine
                    }
                    else
                    {
                        m_pAudio->play(eSound::AKclick);
                    }
                    break;
                case 3:  break;
                case 4:  break;
            }

        }


        if (m_pKeyboard->Down('A'))     //strafe right
        {
            m_pPlayer->StrafeRight();   //set strafe right true in CPlayer::move()
        }

        if (m_pKeyboard->Down('D'))     //strafe left
        {
            m_pPlayer->StrafeLeft();
        }
        if (m_pKeyboard->Down('W'))  //strafe forward
        {
            m_pPlayer->StrafeForward();
        }

        if (m_pKeyboard->Down('S')) //strafe backward
        {
            m_pPlayer->StrafeBackward();
        }

        if (m_pKeyboard->TriggerDown('F')) //toggle focus mode
        {
            if (m_pPlayer->m_bIsFocusing == false)   //if not focusing, start focusing
            {
                m_pPlayer->m_fTimeLastFocus = m_pTimer->GetTime();  //get the time for when the player started focusing
                m_pPlayer->m_bIsFocusing = true;
                m_pAudio->play(eSound::SlowMoStart); //technically could break if past focus time? Is there a way to make this louder?
            }
            else if (m_pPlayer->m_bIsFocusing == true)
            {
                m_pPlayer->m_fTimeLastFocus = 0;    //reset player focus for safety
                m_pPlayer->m_bIsFocusing = false;
                m_pAudio->play(eSound::SlowMoEnd); //Technically could break if past focus time? Is there a way to make this louder?
            }
        }

        if (m_pKeyboard->TriggerDown('R')) // Reload Currently Equiped Gun
        {
            switch (m_pPlayer->m_nWeaponSelector)
            {
                case 0: //The magnum is a revolver, so its ammo must be individually loaded (MASH THAT 'R' KEY!!!!)
                    if (m_pPlayer->m_nRevolverMag < m_pPlayer->m_nRevolverFullMag)  //if gun is not full
                    {
                        m_pAudio->play(eSound::bulletload);
                        m_pPlayer->m_fTimeLastReload = m_pTimer->GetTime(); //get reload time
                        m_pPlayer->m_bIsReloading = true;  //set reloading to true
                        m_pPlayer->m_nRevolverMag += 1; // add 1 bullet to the magazine
                    }
                    break;
                case 1: //The pistol has a magazine, so it must take some time to reload, but it does every round all at once!
                    if (m_pPlayer->m_bIsReloading != true && (m_pPlayer->m_nPistolMag != m_pPlayer->m_nPistolFullMag))  //if the player is already reloading don't reset the timer, and dont reload if the mag is already full
                    {
                        m_pAudio->play(eSound::Pistolreload);
                        m_pPlayer->m_fTimeLastReload = m_pTimer->GetTime(); //get reload time
                        m_pPlayer->m_bIsReloading = true;  //set reloading to true
                    }
                    break;   //pistol is a magazine based reload, so set 'm_bIsReloading' to true
                case 2: //The shotgun has a magazine, so it must take some time to reload, but it does every round all at once!
                    if (m_pPlayer->m_bIsReloading != true && (m_pPlayer->m_nShotgunMag != m_pPlayer->m_nShotgunFullMag))  //if the player is already reloading don't reset the timer, and dont reload if the mag is already full
                    {
                        m_pAudio->play(eSound::AKreload);
                        m_pPlayer->m_fTimeLastReload = m_pTimer->GetTime(); //get reload time
                        m_pPlayer->m_bIsReloading = true;  //set reloading to true
                    }
                    break;
                case 3:  break;
                case 4:  break;
            }
        }

        if (m_pKeyboard->TriggerDown('1')) // Switch to Revolver
        {
            m_pAudio->play(eSound::AKrack);
            m_pPlayer->m_bIsReloading = false;  //cancel reloading if changing to a new gun
            m_pPlayer->m_nWeaponSelector = 0;
            m_pPlayer->UpdatePlayerDamage();    //switched weapon, update damage
        }

        if (m_pKeyboard->TriggerDown('2')) // Switch to Pistol
        {
            m_pAudio->play(eSound::AKrack);
            m_pPlayer->m_bIsReloading = false;  //cancel reloading if changing to a new gun
            m_pPlayer->m_nWeaponSelector = 1;
            m_pPlayer->UpdatePlayerDamage();    //switched weapon, update damage
        }

        if (m_pKeyboard->TriggerDown('3')) // Switch to Shotgun
        {
            m_pAudio->play(eSound::AKrack);
            m_pPlayer->m_bIsReloading = false;  //cancel reloading if changing to a new gun
            m_pPlayer->m_nWeaponSelector = 2;
            m_pPlayer->UpdatePlayerDamage();    //switched weapon, update damage
        }

        if (m_pKeyboard->TriggerDown('4')) // Switch to ______
        {
            //m_pPlayer->UpdatePlayerDamage();    //switched weapon, update damage
            //m_pPlayer->m_bIsReloading = false;  //cancel reloading if changing to a new gun
            //m_pPlayer->m_nWeaponSelector = 3;
        }

        if (m_pKeyboard->TriggerDown('5')) // Switch to ______
        {
            //m_pPlayer->UpdatePlayerDamage();    //switched weapon, update damage
            //m_pPlayer->m_bIsReloading = false;  //cancel reloading if changing to a new gun
            //m_pPlayer->m_nWeaponSelector = 4;
        }

        if (m_pKeyboard->TriggerDown('G')) //toggle god mode
            m_bGodMode = !m_bGodMode;
    } //if
} //KeyboardHandler

/// Poll the XBox controller state and respond to the controls there.

void CGame::ControllerHandler()
{
    if (!m_pController->IsConnected())return;

    m_pController->GetState(); //get state of controller's controls 

    if (m_pPlayer) { //safety
        m_pPlayer->SetSpeed(100 * m_pController->GetRTrigger());
        m_pPlayer->Aim(m_pController->GetRThumb());

        if (m_pController->GetButtonRSToggle()) //fire gun
        {
            switch (m_pPlayer->m_nWeaponSelector)
            {
            case 0:
                if ((m_pPlayer->m_nRevolverMag > 0) && (m_pPlayer->m_bIsReloading == false))  //if gun is not empty
                {
                    m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet);  // fire the gun
                    m_pPlayer->m_nRevolverMag -= 1; // remove 1 bullet from the magazine
                }
                else
                {
                    m_pAudio->play(eSound::Pistolclick);
                }
                break;
            case 1:
                if ((m_pPlayer->m_nPistolMag > 0) && (m_pPlayer->m_bIsReloading == false))  //if gun is not empty
                {
                    m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet);  // fire the gun
                    m_pPlayer->m_nPistolMag -= 1; // remove 1 bullet from the magazine
                }
                else
                {
                    m_pAudio->play(eSound::Pistolclick);
                }
                break;   //pistol is a magazine based reload, so set 'm_bIsReloading' to true
            case 2:
                if ((m_pPlayer->m_nShotgunMag > 0) && (m_pPlayer->m_bIsReloading == false))  //if gun is not empty
                {
                    for (int i = 0; i < m_pPlayer->m_nShotgunPellets; i++)  //fire a bullet for whatever the damage modifier would be instead of actually increasing the damage
                    {
                        m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet);  // fire the gun
                    }

                    m_pPlayer->m_nShotgunMag -= 1; // remove 1 bullet from the magazine
                }
                else
                {
                    m_pAudio->play(eSound::AKclick);
                }
                break;
            case 3:  break;
            case 4:  break;
            }

        }
        
        //Reload
        if (m_pController->GetButtonXToggle())
        {
            switch (m_pPlayer->m_nWeaponSelector)
            {
            case 0: //The magnum is a revolver, so its ammo must be individually loaded (MASH THAT 'R' KEY!!!!)
                if (m_pPlayer->m_nRevolverMag < m_pPlayer->m_nRevolverFullMag)  //if gun is not full
                {
                    m_pAudio->play(eSound::bulletload);
                    m_pPlayer->m_fTimeLastReload = m_pTimer->GetTime(); //get reload time
                    m_pPlayer->m_bIsReloading = true;  //set reloading to true
                    m_pPlayer->m_nRevolverMag += 1; // add 1 bullet to the magazine
                }
                break;
            case 1: //The pistol has a magazine, so it must take some time to reload, but it does every round all at once!
                if (m_pPlayer->m_bIsReloading != true && (m_pPlayer->m_nPistolMag != m_pPlayer->m_nPistolFullMag))  //if the player is already reloading don't reset the timer, and dont reload if the mag is already full
                {
                    m_pAudio->play(eSound::Pistolreload);
                    m_pPlayer->m_fTimeLastReload = m_pTimer->GetTime(); //get reload time
                    m_pPlayer->m_bIsReloading = true;  //set reloading to true
                }
                break;   //pistol is a magazine based reload, so set 'm_bIsReloading' to true
            case 2: //The shotgun has a magazine, so it must take some time to reload, but it does every round all at once!
                if (m_pPlayer->m_bIsReloading != true && (m_pPlayer->m_nShotgunMag != m_pPlayer->m_nShotgunFullMag))  //if the player is already reloading don't reset the timer, and dont reload if the mag is already full
                {
                    m_pAudio->play(eSound::AKreload);
                    m_pPlayer->m_fTimeLastReload = m_pTimer->GetTime(); //get reload time
                    m_pPlayer->m_bIsReloading = true;  //set reloading to true
                }
                break;
            case 3:  break;
            case 4:  break;
            }
        }

        //Start sprinting if you have stamina
        if (m_pController->GetLTrigger() == 1)
        {
            if (m_pPlayer->m_nStamina > 0) {    //if stamina is not empty

                m_pPlayer->m_bIsDashing = true;
            }
            else
            {
                m_pPlayer->SetSpeed(0.0f);
            }
        }

        //Stop sprinting
        if (m_pController->GetLTrigger() == 0)
        {
            m_pPlayer->SetSpeed(0.0f);
            m_pPlayer->m_bIsDashing = false;
        }

        //Toggle focus
        if (m_pController->GetButtonYToggle())
        {
            if (m_pPlayer->m_bIsFocusing == false)   //if not focusing, start focusing
            {
                m_pPlayer->m_fTimeLastFocus = m_pTimer->GetTime();  //get the time for when the player started focusing
                m_pPlayer->m_bIsFocusing = true;
                m_pAudio->play(eSound::SlowMoStart); //technically could break if past focus time? Is there a way to make this louder?
            }
            else if (m_pPlayer->m_bIsFocusing == true)
            {
                m_pPlayer->m_fTimeLastFocus = 0;    //reset player focus for safety
                m_pPlayer->m_bIsFocusing = false;
                m_pAudio->play(eSound::SlowMoEnd); //Technically could break if past focus time? Is there a way to make this louder?
            }
        }

        //Change weapon up.
        if (m_pController->GetButtonBToggle())
        {
            m_pAudio->play(eSound::AKrack);
            m_pPlayer->m_nWeaponSelector++;

            if (m_pPlayer->m_nWeaponSelector > 2)
            {
                m_pPlayer->m_nWeaponSelector = 0;
            }
        }

        //Change weapon down.
        if (m_pController->GetButtonAToggle())
        {
            m_pAudio->play(eSound::AKrack);
            m_pPlayer->m_nWeaponSelector--;

            if (m_pPlayer->m_nWeaponSelector < 0)
            {
                m_pPlayer->m_nWeaponSelector = 2;
            }
        }

        if (m_pController->GetDPadRight()) //strafe right
            m_pPlayer->StrafeLeft();

        if (m_pController->GetDPadLeft()) //strafe left
            m_pPlayer->StrafeRight();

        if (m_pController->GetDPadDown()) //strafe back
            m_pPlayer->StrafeBackward();

        if (m_pController->GetDPadUp()) //strafe back
            m_pPlayer->StrafeForward();
    } //if
} //ControllerHandler

/// Draw the current frame rate to a hard-coded position in the window.
/// The frame rate will be drawn in a hard-coded position using the font
/// specified in `gamesettings.xml`.

void CGame::DrawFrameRateText()
{
    const std::string s = std::to_string(m_pTimer->GetFPS()) + " fps"; //frame rate
    const Vector2 pos(m_nWinWidth - 128.0f, 30.0f); //hard-coded position
    m_pRenderer->DrawScreenText(s.c_str(), pos); //draw to screen
} //DrawFrameRateText

/// Draw the god mode text to a hard-coded position in the window using the
/// font specified in `gamesettings.xml`.

void CGame::DrawGodModeText()
{
    const Vector2 pos(64.0f, 270.0f); //hard-coded position
    m_pRenderer->DrawScreenText("God Mode", pos); //draw to screen
} //DrawGodModeText

void CGame::DrawFocusModeText()
{
    const Vector2 pos(64.0f, 210.0f); //hard-coded position
    m_pRenderer->DrawScreenText("Focusing!!!", pos); //draw to screen
} //DrawFocusModeText 

void CGame::DrawPlayerComboText()
{
    const Vector2 pos(64.0f, 180.0f); //hard-coded position
    std::string combo_text = "COMBO: " + std::to_string(m_pPlayer->m_nCombo); //string for players combo
    m_pRenderer->DrawScreenText(combo_text.c_str(), pos, Colors::Black); //draw player's combo to screen
} //DrawPlayerComboText

void CGame::DrawPlayerDamageText()       //draw player's damage (this is mostly for testing and may be removed later)
{
    const Vector2 pos(64.0f, 150.0f); //hard-coded position
    std::string damage_text = "DAMAGE: " + std::to_string(m_pPlayer->m_nPlayerDamage); //string for players damage
    m_pRenderer->DrawScreenText(damage_text.c_str(), pos, Colors::Black); //draw player's damage to screen
} //DrawPlayerDamageText

void CGame::DrawReloadingText()
{
    const Vector2 pos(64.0f, 240.0f); //hard-coded position
    m_pRenderer->DrawScreenText("Reloading...", pos); //draw to screen
} //DrawReloadingText 

void CGame::DrawRevolverAmmoText()
{
    const Vector2 pos(64.0f, 300.0f); //hard-coded position
    std::string gun_text = "Magnum: " + std::to_string(m_pPlayer->m_nRevolverMag) + "/" + std::to_string(m_pPlayer->m_nRevolverFullMag); //string for players gun ammo
    m_pRenderer->DrawScreenText(gun_text.c_str(), pos, Colors::Black); //draw player's damage to screen
}//DrawRevolverAmmo

void CGame::DrawPistolAmmoText()
{
    const Vector2 pos(64.0f, 300.0f); //hard-coded position
    std::string gun_text = "Pistol: " + std::to_string(m_pPlayer->m_nPistolMag) + "/" + std::to_string(m_pPlayer->m_nPistolFullMag); //string for players damage
    m_pRenderer->DrawScreenText(gun_text.c_str(), pos, Colors::Black); //draw player's damage to screen
}//DrawPistolAmmoText

void CGame::DrawShotgunAmmoText()
{
    const Vector2 pos(64.0f, 300.0f); //hard-coded position
    std::string gun_text = "Shotgun: " + std::to_string(m_pPlayer->m_nShotgunMag) + "/" + std::to_string(m_pPlayer->m_nShotgunFullMag); //string for players damage
    m_pRenderer->DrawScreenText(gun_text.c_str(), pos, Colors::Black); //draw player's damage to screen
}//DrawShotgunAmmoText

void CGame::DrawBarDisplay(eSprite sprite, int p, int i, Vector2 pos)
{
    if (m_pPlayer != NULL)
    { //if the player is alive
        //std::string hp_text = "Heath: " + std::to_string(m_pPlayer->m_nHealth) + "/" + std::to_string(m_pPlayer->m_nMaxHealth); //string for players health
        //m_pRenderer->DrawBoxFilled(64, m_nWinHeight - 50, 100, 100, Vector4(0.0, 0.0, 0.0, 255.0)); //This does not work and I do not know why
        LSpriteDesc2D desc;
        desc.m_nCurrentFrame = p;
        desc.m_nSpriteIndex = (UINT)sprite; //sprite index forsprite index for barSprite
        desc.m_vPos.x = pos.x;
        desc.m_vPos.y = pos.y;
        int width = m_pRenderer->GetWidth(eSprite::Bar);

        for (int j = 0; j <= i; j++)
        {
            m_pRenderer->Draw(&desc);
            //m_pObjectManager->create(sprite, pos);
            desc.m_vPos.x += width + 1.0;
        }
    }
} //DrawPlayerHealthDisplay

void CGame::DrawPlayerHealthText()
{

    const Vector2 pos(64.0f, 30.0f); //hard-coded position
    if (m_pPlayer != NULL)
    { //if the player is alive
        std::string hp_text = "Heath: " + std::to_string(m_pPlayer->m_nHealth) + "/" + std::to_string(m_pPlayer->m_nMaxHealth); //string for players health
        m_pRenderer->DrawScreenText(hp_text.c_str(), pos, Colors::Red); //draw player's Heath to screen
    }

} //DrawPlayerHealthText 

void CGame::DrawPlayerFocusText()
{
    const Vector2 pos(64.0f, 60.0f); //hard-coded position
    if (m_pPlayer != NULL)
    { //if the player is alive
        std::string fp_text = "Focus: " + std::to_string(m_pPlayer->m_nFocus) + "/" + std::to_string(m_pPlayer->m_nMaxFocus); //string for players Focus
        m_pRenderer->DrawScreenText(fp_text.c_str(), pos, Colors::Blue); //draw player's Focus to screen
    }

} //DrawPlayerFocusText 

void CGame::DrawPlayerStaminaText()
{
    const Vector2 pos(64.0f, 90.0f); //hard-coded position
    if (m_pPlayer != NULL)
    { //if the player is alive
        std::string sp_text = "Stamina: " + std::to_string(m_pPlayer->m_nStamina) + "/" + std::to_string(m_pPlayer->m_nMaxStamina); //string for players Stamina
        m_pRenderer->DrawScreenText(sp_text.c_str(), pos, Colors::Green); //draw player's Stamina to screen
    }

} //DrawPlayerStaminaText 

void CGame::DrawDifficultyText()
{
    const Vector2 pos(64.0f, 120.0f); //hard-coded position
    if (m_pPlayer != NULL)
    { //if the player is alive
        std::string sp_text = "Difficulty Mod: " + std::to_string(m_pObjectManager->m_nDifficultyModifier); //string for players Stamina
        m_pRenderer->DrawScreenText(sp_text.c_str(), pos, Colors::Black); //draw player's Stamina to screen
    }

}


/// Ask the object manager to draw the game objects. The renderer is notified of
/// the start and end of the frame so that it can let Direct3D do its
/// pipelining jiggery-pokery.

void CGame::RenderFrame()
{
    m_pRenderer->BeginFrame();
    //m_pRenderer->BeginFrameExt(); //required before rendering

    m_pObjectManager->draw(); //draw objects
    m_pParticleEngine->Draw(); //draw particles
    if (m_bDrawFrameRate)DrawFrameRateText(); //draw frame rate, if required
    if (m_bGodMode)DrawGodModeText(); //draw god mode text, if required

    if (m_pPlayer != nullptr)
    {
        DrawBarDisplay(eSprite::Bar, 0, m_pPlayer->m_nHealth-1, Vector2(20, 1055));//Display Healthbar //Also player is null at the start but fixes itself
        DrawBarDisplay(eSprite::Bar, 1, m_pPlayer->m_nStamina-1, Vector2(20, 52));//Display Staminabar
        DrawBarDisplay(eSprite::Bar, 2, m_pPlayer->m_nFocus-1, Vector2(20, 22));//Display Staminabar
        switch (m_pPlayer->m_nWeaponSelector)//Weapon ammo displayed based on equipped weapon
        {
            case 0: DrawBarDisplay(eSprite::Bar, 3, m_pPlayer->m_nRevolverMag-1, Vector2(20, 1030)); break; //Draw Revolver ammo
            case 1: DrawBarDisplay(eSprite::Bar, 4, m_pPlayer->m_nPistolMag - 1, Vector2(20, 1030)); break; //Draw Pistol Ammo
            case 2: DrawBarDisplay(eSprite::Bar, 5, m_pPlayer->m_nShotgunMag - 1, Vector2(20, 1030)); break; //Draw Shotgun Ammo
            case 3:  break;
            case 4:  break;
        }
        if (m_pPlayer->m_bIsFocusing == true)
            DrawFocusModeText(); //draw focus mode text, if focusing and alive
        if (m_pPlayer->m_bIsReloading == true)
            DrawReloadingText();
        if (m_pPlayer->m_nCombo != 0)
            DrawPlayerComboText();  //if the player's combo is greater than 0, display it
        DrawPlayerDamageText();  //draw player's damage (this is mostly for testing and may be removed later)
        DrawDifficultyText();
        //Picking which weapon ammo to display based on which is currently equipped
        /* Previous Text display
        switch (m_pPlayer->m_nWeaponSelector)
        {
            case 0: DrawRevolverAmmoText(); break;
            case 1: DrawPistolAmmoText(); break;
            case 2: DrawShotgunAmmoText(); break;
            case 3:  break;
            case 4:  break;
        }
        */
    }

    DrawPlayerHealthText(); //draw players current and max health
    DrawPlayerFocusText();  //draw players current and max focus
    DrawPlayerStaminaText(); //draw players current and max stamina


    //DrawPlayerHealthDisplay(); //draw players current and max health
    

    m_pRenderer->EndFrame();
    //m_pRenderer->EndFrameExt(); //required after rendering
} //RenderFrame

/// Make the camera follow the player, but don't let it get too close to the
/// edge unless the world is smaller than the window, in which case we just
/// center everything.

void CGame::FollowCamera()
{
    if (m_pPlayer == nullptr)return; //safety

    Vector3 vCameraPos(m_pPlayer->GetPos()); //player position

    if (m_vWorldSize.x > m_nWinWidth) { //world wider than screen
        vCameraPos.x = std::max(vCameraPos.x, m_nWinWidth / 2.0f); //stay away from the left edge
        vCameraPos.x = std::min(vCameraPos.x, m_vWorldSize.x - m_nWinWidth / 2.0f);  //stay away from the right edge
    } //if
    else vCameraPos.x = m_vWorldSize.x / 2.0f; //center horizontally.

    if (m_vWorldSize.y > m_nWinHeight) { //world higher than screen
        vCameraPos.y = std::max(vCameraPos.y, m_nWinHeight / 2.0f);  //stay away from the bottom edge
        vCameraPos.y = std::min(vCameraPos.y, m_vWorldSize.y - m_nWinHeight / 2.0f); //stay away from the top edge
    } //if
    else vCameraPos.y = m_vWorldSize.y / 2.0f; //center vertically

    m_pRenderer->SetCameraPos(vCameraPos); //camera to player
} //FollowCamera

/// This function will be called regularly to process and render a frame
/// of animation, which involves the following. Handle keyboard input.
/// Notify the audio player at the start of each frame so that it can prevent
/// multiple copies of a sound from starting on the same frame.  
/// Move the game objects. Render a frame of animation. 

void CGame::ProcessFrame()
{
    KeyboardHandler(); //handle keyboard input
    ControllerHandler(); //handle controller input
    m_pAudio->BeginFrame(); //notify audio player that frame has begun

    m_pTimer->Tick([&]() { //all time-dependent function calls should go here
        m_pObjectManager->move(); //move all objects
        FollowCamera(); //make camera follow player
        m_pParticleEngine->step(); //advance particle animation

    });

    RenderFrame(); //render a frame of animation
    ProcessGameState(); //check for end of game
    MusicHandler(); //Handles Music and looping
} //ProcessFrame

void CGame::MusicHandler()
{
    if (!start)
    {
        m_pAudio->loop(eSound::FeroLoop); //play background music
        start = true;
    }
}

/// Take action appropriate to the current game state. If the game is currently
/// playing, then if the player has been killed or all turrets have been
/// killed, then enter the wait state. If the game has been in the wait
/// state for longer than 3 seconds, then restart the game.

void CGame::ProcessGameState()
{
    static float t = 0; //time at start of game
    

    switch (m_eGameState) {
    case eGameState::Playing:
        if (m_pTimer->GetTime() - t > 60.0f && !m_bAnimalControlOfficerSpawned && m_nNextLevel != 9) //Checks if it's been past a minute, if the animal control officer has already spawned, and if the level is not the boss level.
        {
            m_pObjectManager->create(eSprite::AnimalControlOfficer, m_pPlayer->startingPosition);   //Summons the animal control officer after a certain amount of time.
            m_bAnimalControlOfficerSpawned = true;
        }
        if (m_pPlayer == nullptr || m_pObjectManager->GetNumEnemies() == 0) {       //MIGHT REMOVE SECOND COMPARISON

            m_eGameState = eGameState::Waiting; //now waiting
            t = m_pTimer->GetTime(); //start wait timer
        } //if
        break;

    case eGameState::Waiting:
        if (m_pTimer->GetTime() - t > 0.5f)  //0.5 seconds has elapsed since level end
        {
            if ((m_pObjectManager->GetNumEnemies() == 0) && (m_pPlayer != nullptr)) //player won and didn't die                   //HERE IS WHERE THE LEVEL ENDS!!!
            {
                m_pAudio->play(eSound::HappyChord1);    //Victory Sound

                //Save the player's updated stats from powerups
                m_nMaxHealthS = m_pPlayer->m_nMaxHealth;
                m_nMaxFocusS = m_pPlayer->m_nMaxFocus;
                m_nMaxStaminaS = m_pPlayer->m_nMaxStamina;
                m_fMovementSpeedS = m_pPlayer->m_fMovementSpeed;
                m_fMovementSpeedModifierS = m_pPlayer->m_fMovementSpeedModifier;
                m_nDamageUpgradesS = m_pPlayer->m_nDamageUpgrades;
                m_nHealthS = m_pPlayer->m_nHealth;
                m_nFocusS = m_pPlayer->m_nFocus;
                m_nStaminaS = m_pPlayer->m_nStamina;

                m_nWeaponSelectorS = m_pPlayer->m_nWeaponSelector;  //Keep the same weapon equiped

                if (incrementFlag == false)
                    m_nNextLevel = (m_nNextLevel + 1); //advance next level

                if (m_nNextLevel == 10)  //if the level is passed the max level, reset to the level after the first level
                {
                    m_nNextLevel = 0;   //reset to first level
                }
                if (incrementFlag == false)
                    m_pObjectManager->m_nDifficultyModifier++;    //difficulty is increased

                incrementFlag = true;
            }
            else if (m_pPlayer == nullptr)  //player died, reset all their stats and the level back to level 1
            {
                m_pAudio->play(eSound::Octave1);    //Loss Sound
   
                //MAKE SURE TO UPDATE THESE IF THEY ARE CHANGED IN PLAYER.h
                m_nMaxHealthS = 15;
                m_nMaxFocusS = 10;
                m_nMaxStaminaS = 5;
                m_fMovementSpeedS = 150.0f;
                m_fMovementSpeedModifierS = 0.0f;
                m_nDamageUpgradesS = 0;
                m_nHealthS = 15;
                m_nFocusS = 10;
                m_nStaminaS = 5;

                m_nNextLevel = 0;   //reset to first level
                m_pObjectManager->m_nDifficultyModifier = 0;   //reset difficulty

            }
            if (m_pTimer->GetTime() - t > 3.0f) //wait for sound to play first
            {
                m_bAnimalControlOfficerSpawned = false;
                incrementFlag = false;
                BeginGame(); //restart game
            }
        } //if
        break;
    } //switch
} //CheckForEndOfGame