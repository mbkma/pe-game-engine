#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <glm/glm.hpp>
#include <string>

#include "global.h" // for enum
#include "game-object.h"

class btCharacterControllerInterface;
class btKinematicCharacterController;

class PlayerObject : public GameObject
{
public:
    btKinematicCharacterController* m_character;
    class btPairCachingGhostObject* m_ghostObject;

    // constructor/destructor
    PlayerObject(Model *item);

    void createController(btVector3 initialPosition = btVector3(0.0f, 0.0f, 0.0f), btQuaternion initialRotation = btQuaternion(0,0,0,1));

    void ProcessKeyboard(Direction direction, float dt);

    void GetTransform(btScalar* transform) {
        btTransform trans = m_ghostObject->getWorldTransform();
        trans.getOpenGLMatrix(transform);
    }
};

#endif
