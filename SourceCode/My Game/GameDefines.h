/// \file GameDefines.h
/// \brief Game specific defines.

#ifndef __L4RC_GAME_GAMEDEFINES_H__
#define __L4RC_GAME_GAMEDEFINES_H__

#include "Defines.h"

/// \brief Sprite enumerated type.
///
/// An enumerated type for the sprites, which will be cast to an unsigned
/// integer and used for the index of the corresponding texture in graphics
/// memory. `Size` must be last.

enum class eSprite: UINT{
  Background, Bullet, Bullet2, Player, Turret, Smoke, Spark, Tile, Line, Ant, Bar, 
  AntSpriteSheet, Door, BossTurret, AnimalControlOfficer, Ghost, Health, HealthUp, 
  StaminaUp, FocusUp, MovementSpeedUp, DamageUp, MGTurret,
  Size  //MUST BE LAST
}; //eSprite

/// \brief Sound enumerated type.
///
/// An enumerated type for the sounds, which will be cast to an unsigned
/// integer and used for the index of the corresponding sample. `Size` must 
/// be last.

enum class eSound: UINT{
  Start, Boom, Clang, Grunt, Gun, Ricochet, Ow, FeroLoop, SlowMoStart, SlowMoEnd,
  Pistolclick, Pistolreload, AKreload, AKrack, AKclick, bulletload, 
  Bend1, Bend2, Bend3, Chug1, Chug2, PowerChord1, PowerChord2, PowerChord3, PowerChord4, Slide1, Octave1, HappyChord1,
  Size  //MUST BE LAST
}; //eSound

/// \brief Game state enumerated type.
///
/// An enumerated type for the game state, which can be either playing or
/// waiting a short interval for the level to end after winning or losing to
/// let sounds and particle effects play to the end without being cut short.

enum class eGameState{
  Playing, Waiting
}; //eGameState

#endif //__L4RC_GAME_GAMEDEFINES_H__
