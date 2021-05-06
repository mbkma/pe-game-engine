#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <glm/glm.hpp>
#include <string>

#include "physic-object.h"
#include "player.h"

class btCharacterControllerInterface;
class btKinematicCharacterController;

class PlayerObject : public PhysicObject
{
public:
    Player *m_Player;

	btKinematicCharacterController* m_character;
	class	btPairCachingGhostObject* m_ghostObject;

    // constructor/destructor
    PlayerObject(Model *item,
                 const btVector3 &initialPosition,
                 const btQuaternion &initialRotation,
                 Player *player);

    void moveForward(float dt);

    void moveBackward(float dt);

    void moveLeft(float dt);

    void moveRight(float dt);

    void GetTransform(btScalar* transform) {
        btTransform trans = m_ghostObject->getWorldTransform();
        trans.getOpenGLMatrix(transform);;
    }
};

#endif
