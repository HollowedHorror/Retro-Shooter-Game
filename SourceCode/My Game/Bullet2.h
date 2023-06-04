// file Bullet2.h
// This file is the header for Bullet2.cpp which is used for enemy bullet operations

#pragma once

#include "Object.h"

class CBullet2: public CObject
{
	protected:
		virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); // used in collision effect checking.
		virtual void DeathFX(); // used for death effects.

	public:
		CBullet2(eSprite t, const Vector2& p); // Constructor for Bullet2.
};