#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "physic-object.h"

// BallObject holds the state of the Ball object inheriting
// relevant state data from PhysicObject.
class BallObject : public PhysicObject
{
public:
    // constructor(s)
    BallObject(Model *item,
               btCollisionShape* pShape,
               const float &mass,
               const btVector3 &initialPosition,
               const btQuaternion &initialRotation);
};

#endif
