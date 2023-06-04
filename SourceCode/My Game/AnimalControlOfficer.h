/// \file AnimalControlOfficer.h
/// \brief Interface for the Animal Control Officer class CAnimalControlOfficer.

#ifndef __L4RC_GAME_AnimalControlOfficer_H__
#define __L4RC_GAME_AnimalControlOfficer_H__

#include "Object.h"
#include "Common.h"

/// \brief The AnimalControlOfficer object. 
///
/// CAnimalControlOfficer is the abstract representation of an Animal Control Officer object.

class CAnimalControlOfficer : public CObject {
    friend class CObjectManager; ///< Object manager is a friend.

protected:
    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.

public:
    CAnimalControlOfficer(const Vector2&); ///< Constructor. 
    ~CAnimalControlOfficer(); ///< Destructor. 

    virtual void move(); ///< Move AnimalControlOfficer.
    void Follow(const Vector2&);
}; //CAnimalControlOfficer

#endif //__L4RC_GAME_AnimalControlOfficer_H__
