#include "actor.h"

#include <GLFW/glfw3.h>

glm::mat4 Actor::btScalar2mat4(btScalar* matrix)
{
    return glm::mat4(
        matrix[0], matrix[1], matrix[2], matrix[3],
        matrix[4], matrix[5], matrix[6], matrix[7],
        matrix[8], matrix[9], matrix[10], matrix[11],
        matrix[12], matrix[13], matrix[14], matrix[15]);
}

Actor::Actor(std::string id)
{
    ActorId = id;
}

void Actor::AddRenderComponent(Model *item)
{
    m_item = item;
}

void Actor::draw(Shader *shader)
{
    std::cout << "draw: " << ActorId << std::endl;

    shader->Use();

    glm::mat4 model = glm::mat4(1.0f);

    if (m_pMotionState) // Actor has Physics Component
    {
        btScalar transform[16];
        GetTransform(transform);
        model = btScalar2mat4(transform);
    }
    shader->SetMatrix4("model", model);

    if (m_item && m_item->getNumAnimations() > 0)
    {
        std::vector<glm::mat4> Transforms;
        m_item->boneTransform((float)glfwGetTime()*20, Transforms);
        shader->SetMatrix4v("gBones", Transforms);
    }
    if (m_item)
    {
        m_item->draw(shader);
        std::cout << "draw: and m_item " << std::endl;
    }
}

void Actor::AddPhysicComponent(btCollisionShape* pShape, const float &mass, btVector3 initialPosition, btQuaternion initialRotation)
{


}

void Actor::AddPhysicControllerComponent(btVector3 initialPosition, btQuaternion initialRotation)
{
    // TODO Physic add controller
}

void Actor::ProcessKeyboard(Direction direction, float dt)
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
