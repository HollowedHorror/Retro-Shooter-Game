/// \file Player.h
/// \brief Interface for the player object class CPlayer.

#pragma once
#include "Object.h"
#include "Game.h"

/// \brief The player object. 
///
/// The abstract representation of the player object. The player differs from
/// the other objects in the game in that it moves in respond to device inputs.

class CPlayer : public CObject {
protected:

    //player health stuff used to be here, moved them incase they need to be used by other classes

    bool m_bStrafeLeft = false; ///< Strafe left.
    bool m_bStrafeRight = false; ///< Strafe right.
    bool m_bStrafeBackward = false; ///< Strafe backward.
    bool m_bStrafeForward = false; ///< Strafe forward.

    bool m_bDashLeft = false; ///< Dash left.
    bool m_bDashRight = false; ///< Dash right.
    bool m_bDashBackward = false; ///< Dash backward.
    bool m_bDashForward = false; ///< Dash forward.

    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.

public:
    CPlayer(const Vector2& p);      ///< Constructor.

    virtual void move();            ///< Move player object.

    UINT m_nMaxHealth = 15;         //  Maximum Health.        (made not constant so it can be increase by powerups later)
    UINT m_nHealth = m_nMaxHealth;  //  Current Health.

    UINT m_nMaxFocus = 10;          //  Maximum Focus.        (made not constant so it can be increase by powerups later)
    UINT m_nFocus = m_nMaxFocus;    //  Current Focus.

    UINT m_nMaxStamina = 10;         //  Maximum Stamina.        (made not constant so it can be increase by powerups later)
    UINT m_nStamina = m_nMaxStamina;//  Current Stamina.

    UINT m_nCombo = 0;              //  Current Combo.

    float m_fMovementSpeed = 120.0f;// Float for the base movement speed of the player
    float m_fMovementSpeedModifier = 0.0f;// Float used for modifying player's movement speed.

    UINT m_nPlayerDamage = 0;       // Current Damage, default to revolver's base damage.
    UINT m_nDamageUpgrades = 0;     // Number of Damage powerups the player has picked up.

    INT m_nWeaponSelector = 0;      // This is used to select which gun you are currently using (0 - revolver, 1 - pistol, 2 - shotgun, 3 - rifle, 4 - submachinegun)

    UINT m_nRevolverFullMag = 6;            // Size of the full magazine
    UINT m_nRevolverMag = m_nRevolverFullMag;// Current amount of ammo in the magazine
    UINT m_nRevolverDamage = 9;             // Base damage of the revolver

    UINT m_nPistolFullMag = 17;             // Size of the full magazine
    UINT m_nPistolMag = m_nPistolFullMag;   // Current amount of ammo in the magazine
    UINT m_nPistolDamage = 3;               // Base damage of the pistol
    float m_fPistolReloadTime = 2.5f;       // Base pistol reload speed

    UINT m_nShotgunFullMag = 10;             // Size of the full magazine
    UINT m_nShotgunMag = m_nShotgunFullMag; // Current amount of ammo in the magazine
    UINT m_nShotgunDamage = 3;              // Base damage of the Shotgun
    float m_fShotgunReloadTime = 3.0f;      // Base Shotgun reload speed
    UINT m_nShotgunPellets = 3;             // Number of pellets fired, modified in the UpdatePlayerDamage() instead of the damage

    //TIME FLOATS
    float m_fTimeLastDash = 0.0f;       //used for tracking how long the player has been sprinting
    float m_fTimeLastFocus = 0.0f;      //used for tracking how long the player has been focusing
    float m_fTimeLastHit = 0.0f;        //used for grace period in the player's hit combo.
    float m_fTimeLastReload = 0.0f;     //used for tracking how fast the player reloads their gun
    float m_fTimeLastFire = 0.0f;       //used for tracking how fast the player fires their gun
    float m_fTimeLastGhostHit = 0.0f;   //used to slow down the life drain of ghosts.
    //ACTION CHECKING
    bool m_bIsReloading = false;        //used for if the player is reloading
    bool m_bIsDashing = false;          //used for if the player is sprinting/dashing
    bool m_bIsFocusing = false;         //used for if the player is focusing (bullet time)
    //bool m_bIsBlocking = false;       //used for if the player is blocking (godMode but forced stationary and can't shoot)

    Vector2 startingPosition;               //The position the player starts at.

    void SetSpeed(const float speed);       ///< Set speed.
    void SetRotSpeed(const float speed);    ///< Set rotational velocity.

    void UpdatePlayerDamage(); // used for modifying player's damage from powerups and the combo

    void StrafeLeft();      ///< Strafe left.
    void StrafeRight();     ///< Strafe right.
    void StrafeBackward();  ///< Strafe backward.
    void StrafeForward();   ///< Strafe forward.

    const Vector2& GetPos() const; ///< Get position.
    void Aim(Vector2);  //A function to change the roll of the player for aiming.

}; //CPlayer

