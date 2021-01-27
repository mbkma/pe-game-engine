#include "game-object.h"

GameObject::GameObject(Model item,
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
    transform.setRotation(initialRotation);

    // create the motion state from the
    // initial transform
    m_pMotionState = new MotionState(transform);

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

        cInfo.m_restitution = 0.9f;
        cInfo.m_friction = 1.5f;

    // create the rigid body
    m_pBody = new btRigidBody(cInfo);
}

void GameObject::Draw(Renderer &renderer, btScalar *transform)
{
    switch(this->m_pShape->getShapeType())
    {
        case BOX_SHAPE_PROXYTYPE:
        {
            renderer.Draw(this->Item, transform, 1.0f);
            break;
        }
        case SPHERE_SHAPE_PROXYTYPE:
        {
            // assume the shape is a sphere and typecast it
            const btSphereShape* sphere = static_cast<const btSphereShape*>(this->m_pShape);
            // get the sphere's size from the shape
            float radius = sphere->getMargin();
            // draw the sphere
            renderer.Draw(this->Item, transform, radius);
            break;
        }
        default:
            // unsupported type
            break;
    }
}
