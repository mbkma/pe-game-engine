

#include "physics.h"
#include "game.h"

#include "player-object.h"

PlayerObject::PlayerObject(Model *item,
                           const btVector3 &initialPosition,
                           const btQuaternion &initialRotation,
                           Player *player)

                          :PhysicObject()
{
    Item = item;

    // create the initial transform
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(initialPosition);
    transform.setRotation(initialRotation);

    m_ghostObject = new btPairCachingGhostObject();
    m_ghostObject->setWorldTransform(transform);


    btScalar characterHeight=1.8;
    btScalar characterWidth =0.3;

    btConvexShape* capsule = new btCapsuleShape(characterWidth,characterHeight);
    m_ghostObject->setCollisionShape (capsule);
    m_ghostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);

    m_pShape = capsule;

    btScalar stepHeight = btScalar(0.1);
    m_character = new btKinematicCharacterController (m_ghostObject,capsule,stepHeight);
    m_character->setUp(btVector3(0,1,0));

    m_Player = player;
}

void PlayerObject::ProcessKeyboard(Direction direction, float dt)
{
    ///set walkDirection for our character
    btTransform xform;
    xform = m_ghostObject->getWorldTransform ();

    btVector3 forwardDir = -xform.getBasis()[2];
    btVector3 upDir = xform.getBasis()[1];
    btVector3 strafeDir = xform.getBasis()[0];
    forwardDir.normalize ();
    upDir.normalize ();
    strafeDir.normalize ();

    btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
    btScalar walkVelocity = btScalar(4.1) * 1.0; // 4 km/h -> 1.1 m/s
    btScalar walkSpeed = walkVelocity * dt;

    if (direction == UP)
        walkDirection += forwardDir;
    if (direction == DOWN)
        walkDirection -= forwardDir;
    if (direction == LEFT)
        walkDirection -= strafeDir;
    if (direction == RIGHT)
        walkDirection += strafeDir;

    m_character->setWalkDirection(walkDirection*walkSpeed);
}
