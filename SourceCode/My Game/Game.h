/// \file Game.h
/// \brief Interface for the game class CGame.

#ifndef __L4RC_GAME_GAME_H__
#define __L4RC_GAME_GAME_H__

#include "Component.h"
#include "Common.h"
#include "ObjectManager.h"
#include "Settings.h"
#include "Player.h"

/// \brief The game class.
///
/// The game class is the object-oriented implementation of the game. This class
/// must contain the following public member functions. `Initialize()` does
/// initialization and will be run exactly once at the start of the game.
/// `ProcessFrame()` will be called once per frame to create and render the
/// next animation frame. `Release()` will be called at game exit but before
/// any destructors are run.

class CGame :
    public LComponent,
    public LSettings,
    public CCommon {

private:
    bool incrementFlag = false; //flag used for preventing incrementing values from incrementing more than once when winning
    bool m_bDrawFrameRate = false; ///< Draw the frame rate.
    eGameState m_eGameState = eGameState::Playing; ///< Game state.
    int m_nNextLevel = 0; ///< Current level number.

    //Player Stat Values (placed here so they may be saved through level transitions)
    UINT m_nMaxHealthS = 15; //  Maximum Health. 
    UINT m_nHealthS = m_nMaxHealthS; //  Current Health.
    UINT m_nMaxFocusS = 10; //  Maximum Focus.
    UINT m_nFocusS = m_nMaxFocusS; //  Current Focus.
    UINT m_nMaxStaminaS = 10; //  Maximum Stamina.
    UINT m_nStaminaS = m_nMaxStaminaS; //  Current Stamina.
    float m_fMovementSpeedS = 120.0f;    // Float used for the base value of the player's movement speed.
    float m_fMovementSpeedModifierS = 0.0f;  // Float used for modifying player's movement speed.
    UINT m_nDamageUpgradesS = 0; // Number of Damage powerups the player has picked up.
    UINT m_nWeaponSelectorS = 0; // This is used to select which gun you are currently using (0 - revolver, 1 - pistol, 2 - shotgun, 3 - rifle, 4 - submachinegun)

    void LoadImages(); ///< Load images.
    void LoadSounds(); ///< Load sounds.
    void BeginGame(); ///< Begin playing the game.
    void KeyboardHandler(); ///< The keyboard handler.
    void ControllerHandler(); ///< The controller handler.
    void RenderFrame(); ///< Render an animation frame.
    void DrawFrameRateText(); ///< Draw frame rate text to screen.
    void DrawGodModeText(); ///< Draw god mode text if in god mode.
    void DrawFocusModeText(); //Draw focus mode text if player is focusing.

    void DrawDifficultyText();   //Draws difficulty
    void DrawPlayerHealthText(); ///< Draw player's health text
    void DrawPlayerFocusText(); // Draw player's focus text
    void DrawReloadingText(); // Draw text so player knows that they are reloading!
    void DrawPlayerStaminaText(); ///< Draw player's stamina text
    void DrawPlayerComboText(); // Draw player's combo text
    void DrawPlayerDamageText(); // Draw player's damage text (this is mostly for testing purposes and may be removed later)
    void DrawRevolverAmmoText(); // Draw player's Revolver max and current ammo
    void DrawPistolAmmoText();  // Draw player's Pistol max and current ammo
    void DrawShotgunAmmoText(); // Draw player's Shotgun max and current ammo
    //void DrawPlayerHealthDisplay(); ///< Draw player's health bar
    void DrawBarDisplay(eSprite t, int p, int i, Vector2 pos);

    void CreateObjects(); ///< Create game objects.
    void FollowCamera(); ///< Make camera follow player character.
    void ProcessGameState(); ///< Process game state.
    void MusicHandler(); ///< Process Music and loop
    bool start = false;

public:
    
    ~CGame(); ///< Destructor.

    void Initialize(); ///< Initialize the game.
    void ProcessFrame(); ///< Process an animation frame.
    void Release(); ///< Release the renderer.
}; //CGame

#endif //__L4RC_GAME_GAME_H__
