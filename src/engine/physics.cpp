#include "physics.h"

Physics::Physics(std::vector<GameObject*> GameObjects) : m_GameObjects(GameObjects)
{
    // create the collision configuration
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
    // create the dispatcher
    m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
    // create the broadphase
    m_pBroadphase = new btDbvtBroadphase();
    // create the constraint solver
    m_pSolver = new btSequentialImpulseConstraintSolver();
    // create the world
    m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);
}

Physics::~Physics()
{
    delete m_pWorld;
    delete m_pSolver;
    delete m_pBroadphase;
    delete m_pDispatcher;
    delete m_pCollisionConfiguration;
}

void Physics::CheckForCollisionEvents() {
    // keep a list of the collision pairs we
    // found during the current update
    CollisionPairs pairsThisUpdate;

    // iterate through all of the manifolds in the dispatcher
    for (int i = 0; i < m_pDispatcher->getNumManifolds(); ++i) {
        // get the manifold
        btPersistentManifold* pManifold = m_pDispatcher->getManifoldByIndexInternal(i);

        // ignore manifolds that have
        // no contact points.
        if (pManifold->getNumContacts() > 0) {

            // get the two rigid bodies involved in the collision
            const btRigidBody* pBody0 = static_cast<const btRigidBody*>(pManifold->getBody0());
            const btRigidBody* pBody1 = static_cast<const btRigidBody*>(pManifold->getBody1());

            // always create the pair in a predictable order
            // (use the pointer value..)
            bool const swapped = pBody0 > pBody1;
            const btRigidBody* pSortedBodyA = swapped ? pBody1 : pBody0;
            const btRigidBody* pSortedBodyB = swapped ? pBody0 : pBody1;

            // create the pair
            CollisionPair thisPair = std::make_pair(pSortedBodyA, pSortedBodyB);

            // insert the pair into the current list
            pairsThisUpdate.insert(thisPair);

            // if this pair doesn't exist in the list
            // from the previous update, it is a new
            // pair and we must send a collision event
            if (m_pairsLastUpdate.find(thisPair) == m_pairsLastUpdate.end()) {
                CollisionEvent((btRigidBody*)pBody0, (btRigidBody*)pBody1);
            }
        }
    }

    // create another list for pairs that
    // were removed this update
    CollisionPairs removedPairs;

    // this handy function gets the difference beween
    // two sets. It takes the difference between
    // collision pairs from the last update, and this
    // update and pushes them into the removed pairs list
    std::set_difference( m_pairsLastUpdate.begin(), m_pairsLastUpdate.end(),
    pairsThisUpdate.begin(), pairsThisUpdate.end(),
    std::inserter(removedPairs, removedPairs.begin()));

    // iterate through all of the removed pairs
    // sending separation events for them
    for (CollisionPairs::const_iterator iter = removedPairs.begin(); iter != removedPairs.end(); ++iter) {
        SeparationEvent((btRigidBody*)iter->first, (btRigidBody*)iter->second);
    }

    // in the next iteration we'll want to
    // compare against the pairs we found
    // in this iteration
    m_pairsLastUpdate = pairsThisUpdate;
}

void Physics::CollisionEvent(btRigidBody * pBody0, btRigidBody * pBody1) {
    // find the two colliding objects
    GameObject* pObj0 = FindGameObject(pBody0);
    GameObject* pObj1 = FindGameObject(pBody1);

    // exit if we didn't find anything
    if (!pObj0 || !pObj1) return;

//    std::cout << "CollisionEvent!" << std::endl;
}

void Physics::SeparationEvent(btRigidBody * pBody0, btRigidBody * pBody1) {
    // get the two separating objects
    GameObject* pObj0 = FindGameObject((btRigidBody*)pBody0);
    GameObject* pObj1 = FindGameObject((btRigidBody*)pBody1);

    // exit if we didn't find anything
    if (!pObj0 || !pObj1) return;

//    std::cout << "SeparationEvent!" << std::endl;
}

GameObject* Physics::FindGameObject(btRigidBody* pBody) {
    // search through our list of gameobjects finding
    // the one with a rigid body that matches the given one
    for (auto iter : m_GameObjects) {
        if (iter->GetRigidBody() == pBody) {
            // found the body, so return the corresponding game object
            return iter;
        }
    }
    return 0;
}
