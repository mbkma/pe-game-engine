#include "physic-object.h"

PhysicObject::PhysicObject(Model *item,
                       btCollisionShape* pShape,
                       const float &mass,
                       const btVector3 &initialPosition,
                       const btQuaternion &initialRotation)
                      :Item(item)
{
    // store the shape for later usage
    m_pShape = pShape;

    // create the initial transform
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(initialPosition);
//    transform.setRotation(initialRotation);

    // create the motion state from the
    // initial transform
    m_pMotionState = new btDefaultMotionState(transform);

    // calculate the local inertia
    btVector3 localInertia(0,0,0);

    // objects of infinite mass can't
    // move or rotate
    if (mass != 0.0f) {
        pShape->calculateLocalInertia(mass, localInertia);
    }

    // create the rigid body construction
    // info using the mass, motion state
    // and shape
    btRigidBody::btRigidBodyConstructionInfo cInfo(mass, m_pMotionState, pShape, localInertia);

    cInfo.m_restitution = 0.8f;
    cInfo.m_friction = 1.5f;

    // create the rigid body
    m_pBody = new btRigidBody(cInfo);
}

void PhysicObject::Draw(Renderer &renderer, btScalar *transform)
{
    renderer.Draw(Item, transform);
}
