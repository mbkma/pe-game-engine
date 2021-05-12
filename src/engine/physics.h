#ifndef PHYSICS_H
#define PHYSICS_H

#include <set>

#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

#include "camera.h"
#include "actor.h"

class Physics
{

public:
    Physics();
    ~Physics();

    Camera *m_pCamera;

    // core Bullet components
    btBroadphaseInterface* m_pBroadphase;
    btCollisionConfiguration* m_pCollisionConfiguration;
    btCollisionDispatcher* m_pDispatcher;
    btConstraintSolver* m_pSolver;
    btDynamicsWorld* m_pWorld;


    btCollisionShape*  m_pShape{};
    btRigidBody*       m_pBody{};
    btMotionState*       m_pMotionState{};

    btCollisionShape* GetShape() { return m_pShape; }

    btRigidBody* GetRigidBody() { return m_pBody; }

    btMotionState* GetMotionState() { return m_pMotionState; }



    // convenient typedefs for collision events
    typedef std::pair<const btRigidBody*, const btRigidBody*> CollisionPair;
    typedef std::set<CollisionPair> CollisionPairs;

    // collision event variables
    CollisionPairs m_pairsLastUpdate;

    struct RayResult {
        btRigidBody* pBody;
        btVector3 hitPoint;
    };

    btRigidBody* pBody;

    void CheckForCollisionEvents();
    void CollisionEvent(btRigidBody * pBody0, btRigidBody * pBody1);
    void SeparationEvent(btRigidBody * pBody0, btRigidBody * pBody1);
    bool Raycast(const btVector3 &startPosition, const btVector3 &direction, btRigidBody* pBody);
    btVector3 GetPickingRay(int x, int y);
    void Shoot(const btVector3 &direction, btRigidBody *body);

    Actor* FindActorByPhysic(btRigidBody* pBody);
};

#endif
