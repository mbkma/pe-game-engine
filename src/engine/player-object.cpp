

#include "physics.h"
#include "game.h"

#include "player-object.h"

PlayerObject::PlayerObject(Model *item,
                           const btVector3 &initialPosition,
                           const btQuaternion &initialRotation,
                           Player *player)

                          :PhysicObject()
{
    int mass = 1;

    Item = item;

    // create the initial transform
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(initialPosition);
    transform.setRotation(initialRotation);

    m_ghostObject = new btPairCachingGhostObject();
    m_ghostObject->setWorldTransform(transform);


    btScalar characterHeight=0.2;
    btScalar characterWidth =0.5;

    btConvexShape* capsule = new btCapsuleShape(characterWidth,characterHeight);
    m_ghostObject->setCollisionShape (capsule);
    m_ghostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);

    m_pShape = capsule;

    btScalar stepHeight = btScalar(0.75);
    m_character = new btKinematicCharacterController (m_ghostObject,capsule,stepHeight);
    m_character->setUp(btVector3(0,1,0));

    m_Player = player;
}

void PlayerObject::moveForward(float dt)
{
    ///set walkDirection for our character
    btTransform xform;
    xform = m_ghostObject->getWorldTransform ();

    btVector3 forwardDir = -xform.getBasis()[2];
    printf("forwardDir=%f,%f,%f\n",forwardDir[0],forwardDir[1],forwardDir[2]);
    btVector3 upDir = xform.getBasis()[1];
    btVector3 strafeDir = xform.getBasis()[0];
    forwardDir.normalize ();
    upDir.normalize ();
    strafeDir.normalize ();

    btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
    btScalar walkVelocity = btScalar(1.1) * 1.0; // 4 km/h -> 1.1 m/s
    btScalar walkSpeed = walkVelocity * dt;

        walkDirection += forwardDir;

    m_character->setWalkDirection(walkDirection*walkSpeed);
}

void PlayerObject::moveBackward(float dt)
{
    ///set walkDirection for our character
    btTransform xform;
    xform = m_ghostObject->getWorldTransform ();

    btVector3 forwardDir = -xform.getBasis()[2];
    printf("forwardDir=%f,%f,%f\n",forwardDir[0],forwardDir[1],forwardDir[2]);
    btVector3 upDir = xform.getBasis()[1];
    btVector3 strafeDir = xform.getBasis()[0];
    forwardDir.normalize ();
    upDir.normalize ();
    strafeDir.normalize ();

    btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
    btScalar walkVelocity = btScalar(1.1) * 1.0; // 4 km/h -> 1.1 m/s
    btScalar walkSpeed = walkVelocity * dt;

    walkDirection -= forwardDir;

    m_character->setWalkDirection(walkDirection*walkSpeed);
}

void PlayerObject::moveLeft(float dt)
{
    ///set walkDirection for our character
    btTransform xform;
    xform = m_ghostObject->getWorldTransform ();

    btVector3 forwardDir = -xform.getBasis()[2];
    printf("forwardDir=%f,%f,%f\n",forwardDir[0],forwardDir[1],forwardDir[2]);
    btVector3 upDir = xform.getBasis()[1];
    btVector3 strafeDir = xform.getBasis()[0];
    forwardDir.normalize ();
    upDir.normalize ();
    strafeDir.normalize ();

    btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
    btScalar walkVelocity = btScalar(1.1) * 1.0; // 4 km/h -> 1.1 m/s
    btScalar walkSpeed = walkVelocity * dt;

    walkDirection -= strafeDir;

    m_character->setWalkDirection(walkDirection*walkSpeed);
}

void PlayerObject::moveRight(float dt)
{
    ///set walkDirection for our character
    btTransform xform;
    xform = m_ghostObject->getWorldTransform ();

    btVector3 forwardDir = -xform.getBasis()[2];
    printf("forwardDir=%f,%f,%f\n",forwardDir[0],forwardDir[1],forwardDir[2]);
    btVector3 upDir = xform.getBasis()[1];
    btVector3 strafeDir = xform.getBasis()[0];
    forwardDir.normalize ();
    upDir.normalize ();
    strafeDir.normalize ();

    btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
    btScalar walkVelocity = btScalar(1.1) * 1.0; // 4 km/h -> 1.1 m/s
    btScalar walkSpeed = walkVelocity * dt;

    walkDirection += strafeDir;

    m_character->setWalkDirection(walkDirection*walkSpeed);
}
