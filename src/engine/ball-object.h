#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game-object.h"

// BallObject holds the state of the Ball object inheriting
// relevant state data from GameObject.
class BallObject : public GameObject
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
