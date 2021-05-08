#ifndef PHYSICS_H
#define PHYSICS_H

#include <set>

#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

#include "physic-object.h"
#include "camera.h"


class Physics
{

public:
    Physics(Camera *camera);
    ~Physics();

    Camera *m_pCamera;

    // core Bullet components
    btBroadphaseInterface* m_pBroadphase;
    btCollisionConfiguration* m_pCollisionConfiguration;
    btCollisionDispatcher* m_pDispatcher;
    btConstraintSolver* m_pSolver;
    btDynamicsWorld* m_pWorld;

    // convenient typedefs for collision events
    typedef std::pair<const btRigidBody*, const btRigidBody*> CollisionPair;
    typedef std::set<CollisionPair> CollisionPairs;

    // collision event variables
    CollisionPairs m_pairsLastUpdate;

    struct RayResult {
        btRigidBody* pBody;
        btVector3 hitPoint;
    };

    void CheckForCollisionEvents();
    void CollisionEvent(btRigidBody * pBody0, btRigidBody * pBody1);
    void SeparationEvent(btRigidBody * pBody0, btRigidBody * pBody1);
    bool Raycast(const btVector3 &startPosition, const btVector3 &direction, RayResult &output);
    btVector3 GetPickingRay(int x, int y);
    void Shoot(const btVector3 &direction, btRigidBody *body);
};

#endif
