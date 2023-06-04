//PowerUp.h

#pragma once
#include "Object.h"

// Class for dealing with powerups
// Power ups will appear randomly when enemies die, and when touched by the player,
// will provide a different benefit, such as increasing stats, or giving a temporary ability.
// Guns may be unlocked/picked up through this feature as well...
class CPowerUp : public CObject 
{
	protected:
		virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
		//virtual void DeathFX(); ///< Death special effects.

	public:
		CPowerUp(eSprite t, const Vector2& p); ///< Constructor.
}; //CPowerUp.h