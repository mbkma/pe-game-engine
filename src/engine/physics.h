#ifndef PHYSICS_H
#define PHYSICS_H
#include "game-object.h"
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

class Physics
{
public:
    Physics(std::vector<GameObject*> GameObjects);
    ~Physics();

    // core Bullet components
    btBroadphaseInterface* m_pBroadphase;
    btCollisionConfiguration* m_pCollisionConfiguration;
    btCollisionDispatcher* m_pDispatcher;
    btConstraintSolver* m_pSolver;
    btDynamicsWorld* m_pWorld;

    std::vector<GameObject*> m_GameObjects;

    // convenient typedefs for collision events
    typedef std::pair<const btRigidBody*, const btRigidBody*> CollisionPair;
    typedef std::set<CollisionPair> CollisionPairs;

    // collision event variables
    CollisionPairs m_pairsLastUpdate;

    void CheckForCollisionEvents();
    void CollisionEvent(btRigidBody * pBody0, btRigidBody * pBody1);
    void SeparationEvent(btRigidBody * pBody0, btRigidBody * pBody1);
    GameObject* FindGameObject(btRigidBody* pBody);
};

#endif
