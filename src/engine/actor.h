#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "global.h" // for enum
#include "model.h"


class btCharacterControllerInterface;
class btKinematicCharacterController;

class Actor
{
public:
    Actor(std::string id);
    ~Actor(void);

    std::string ActorId{};


    // Render Component
    void AddRenderComponent(Model *item);

    Model *m_item{};

    void draw(Shader *shader);


    // Important! You can only add a Physic Component OR a Physic Controller Component. NOT both!

    // Physic Component
    void AddPhysicComponent(btCollisionShape* pShape, const float &mass, btVector3 initialPosition = btVector3(0.0f, 0.0f, 0.0f), btQuaternion initialRotation = btQuaternion(0,0,0,1));



    void GetTransform(btScalar* transform) {
        if (m_ghostObject)
        {
            btTransform trans = m_ghostObject->getWorldTransform();
            trans.getOpenGLMatrix(transform);
        }
        else if (m_pMotionState)
        {
            btTransform trans;
            m_pMotionState->getWorldTransform(trans);
            trans.getOpenGLMatrix(transform);
        }
    }


    // Physic Controller Component
    void AddPhysicControllerComponent(btVector3 initialPosition = btVector3(0.0f, 0.0f, 0.0f), btQuaternion initialRotation = btQuaternion(0,0,0,1));

    btKinematicCharacterController* m_character{};
    class btPairCachingGhostObject* m_ghostObject{};

    void createController(btVector3 initialPosition = btVector3(0.0f, 0.0f, 0.0f), btQuaternion initialRotation = btQuaternion(0,0,0,1));

    void ProcessKeyboard(Direction direction, float dt);


private:
    glm::mat4 btScalar2mat4(btScalar* matrix);
};




#endif
