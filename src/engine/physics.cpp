#include "physics.h"
#include "resource-manager.h"

// The Width of the screen
const unsigned int SCREEN_WIDTH = 1600;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 900;


const float FAR_PLANE = 100.0f;
const float NEAR_PLANE = 0.1f;

const float FOV = 45.0f;

btVector3 glmTobtVector3(glm::vec3 vec) {
    return btVector3(
        vec[0], vec[1], vec[2]);
}


Physics::Physics(Camera *camera) : m_pCamera(camera)
{
    // create the collision configuration
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
    // create the dispatcher
    m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
    // create the broadphase
//    m_pBroadphase = new btDbvtBroadphase();

	btVector3 worldMin(-1000,-1000,-1000);
	btVector3 worldMax(1000,1000,1000);
	m_pBroadphase = new btAxisSweep3(worldMin,worldMax);

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
    PhysicObject* pObj0 = ResourceManager::FindPhysicObject(pBody0);
    PhysicObject* pObj1 = ResourceManager::FindPhysicObject(pBody1);

    // exit if we didn't find anything
    if (!pObj0 || !pObj1) return;

//    std::cout << "CollisionEvent!" << std::endl;
}

void Physics::SeparationEvent(btRigidBody * pBody0, btRigidBody * pBody1) {
    // get the two separating objects
    PhysicObject* pObj0 = ResourceManager::FindPhysicObject((btRigidBody*)pBody0);
    PhysicObject* pObj1 = ResourceManager::FindPhysicObject((btRigidBody*)pBody1);

    // exit if we didn't find anything
    if (!pObj0 || !pObj1) return;

//    std::cout << "SeparationEvent!" << std::endl;
}

bool Physics::Raycast(const btVector3 &startPosition, const btVector3 &direction, RayResult &output)
{
    if (!m_pWorld)
        return false;

    // get the picking ray from where we clicked
    btVector3 rayTo = direction;
    btVector3 rayFrom = glmTobtVector3(m_pCamera->Position);

    // create our raycast callback object
    btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom,rayTo);

    // perform the raycast
    m_pWorld->rayTest(rayFrom,rayTo,rayCallback);

    std::cout << "hit?" << std::endl;

    // did we hit something?
    if (rayCallback.hasHit())
    {

    std::cout << "HIT..." << std::endl;

        // if so, get the rigid body we hit
        btRigidBody* pBody = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
        if (!pBody)
            return false;

        // prevent us from picking objects
        // like the ground plane
        if (pBody->isStaticObject() || pBody->isKinematicObject())
            return false;

    std::cout << "HIT!" << std::endl;

        // set the result data
        output.pBody = pBody;
        output.hitPoint = rayCallback.m_hitPointWorld;
        return true;
    }

    // we didn't hit anything
    return false;
}

btVector3 Physics::GetPickingRay(int x, int y)
{
    std::cout << "x " << x << "y " << y << std::endl;


    // calculate the field-of-view
    float tanFov = 1.0f / NEAR_PLANE;
    float fov = btScalar(2.0) * btAtan(tanFov);

    // get a ray pointing forward from the
    // camera and extend it to the far plane
    btVector3 rayFrom = glmTobtVector3(m_pCamera->Position);
    btVector3 rayForward = -glmTobtVector3(m_pCamera->Position); // ???
    rayForward.normalize();
    rayForward*= FAR_PLANE;

    // find the horizontal and vertical vectors
    // relative to the current camera view
    btVector3 ver(0.0f, 1.0f, 0.0f);
    btVector3 hor = rayForward.cross(ver);
    hor.normalize();
    ver = hor.cross(rayForward);
    ver.normalize();
    hor *= 2.f * FAR_PLANE * tanFov;
    ver *= 2.f * FAR_PLANE * tanFov;

    // calculate the aspect ratio
    btScalar aspect = SCREEN_WIDTH / (btScalar)SCREEN_HEIGHT;

    // adjust the forward-ray based on
    // the X/Y coordinates that were clicked
    hor*=aspect;
    btVector3 rayToCenter = rayFrom + rayForward;
    btVector3 dHor = hor * 1.f/float(SCREEN_WIDTH);
    btVector3 dVert = ver * 1.f/float(SCREEN_HEIGHT);
    btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * ver;
    rayTo += btScalar(x) * dHor;
    rayTo -= btScalar(y) * dVert;

    std::cout << "rayto (" << rayTo.getX() << ", " << rayTo.getY() << ", " << rayTo.getZ() << ")" << std::endl;

    // return the final result
    return rayTo;
}

void Physics::Shoot(const btVector3 &direction, btRigidBody *body)
{
    // calculate the velocity
    btVector3 velocity = direction;
    velocity.normalize();
    velocity *= 25.0f;

    std::cout << "Test!" << std::endl;

    // set the linear velocity of the box
    body->setLinearVelocity(velocity);
}
