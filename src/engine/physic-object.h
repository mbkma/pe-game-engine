#ifndef PHYSIC_OBJECT_H
#define PHYSIC_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

#include "physics.h"
#include "renderer.h"

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within PhysicObject.
class PhysicObject
{
public:
    Model       *Item;

    btCollisionShape*  m_pShape;
    btRigidBody*       m_pBody;
    btMotionState*       m_pMotionState;

    PhysicObject(){};

    PhysicObject(Model *item,
               btCollisionShape* pShape,
               const float &mass,
               const btVector3 &initialPosition = btVector3(0,0,0),
               const btQuaternion &initialRotation = btQuaternion(0,0,1,1));

    ~PhysicObject() = default;

    // accessors
    btCollisionShape* GetShape() { return m_pShape; }

    btRigidBody* GetRigidBody() { return m_pBody; }

    btMotionState* GetMotionState() { return m_pMotionState; }

    void GetTransform(btScalar* transform) {
        btTransform trans;
        m_pMotionState->getWorldTransform(trans);
        trans.getOpenGLMatrix(transform);;
    }

    // draw
    void Draw(Renderer &renderer, btScalar* transform);
};

#endif
