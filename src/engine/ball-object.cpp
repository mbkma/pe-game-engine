#include "ball-object.h"

BallObject::BallObject(Model *item,
                       btCollisionShape* pShape,
                       const float &mass,
                       const btVector3 &initialPosition,
                       const btQuaternion &initialRotation)
           :PhysicObject(item,
                       pShape,
                       mass,
                       initialPosition,
                       initialRotation)
{

}
