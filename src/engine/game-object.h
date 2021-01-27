#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

#include "renderer.h"
#include "motion-state.h"

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    Model       Item;

    btCollisionShape*  m_pShape;
    btRigidBody*       m_pBody;
    MotionState*       m_pMotionState;

    GameObject(Model item,
               btCollisionShape* pShape,
               const float &mass,
               const btVector3 &initialPosition = btVector3(0,0,0),
               const btQuaternion &initialRotation = btQuaternion(0,0,1,1));

    virtual ~GameObject() = default;

    // accessors
    btCollisionShape* GetShape() { return m_pShape; }

    btRigidBody* GetRigidBody() { return m_pBody; }

    btMotionState* GetMotionState() { return m_pMotionState; }

    void GetTransform(btScalar* transform) {
        if (m_pMotionState) m_pMotionState->GetWorldTransform(transform);
    }

    // draw
    virtual void Draw(Renderer &renderer, btScalar* transform);
};

#endif
