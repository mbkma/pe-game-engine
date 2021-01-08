#include "ball-object.h"

#define GRAVITY 9.81

BallObject::BallObject(glm::vec3 pos, Model item, float radius, glm::vec3 velocity)
    : GameObject(pos, item, radius, velocity), Radius(radius)
{

}

glm::vec3 BallObject::Move(float dt, unsigned int window_width)
{
    if (this->Position.y <= 0)
    {
        if (-this->Velocity.y <= 6*dt*GRAVITY)
        {
            this->Position.y = 0;
            this->Velocity.y = 0;
        }
        else
        {
            this->Velocity.y = 0.7*glm::sqrt(glm::abs((glm::pow(this->Velocity.y,2)+2.0f*GRAVITY*this->Velocity.y*dt)));
            this->Position.y = -this->Position.y;
        }
    }
    else
    {
        this->Position += this->Velocity * dt;
        this->Velocity.y -= GRAVITY * dt;
        this->Velocity.x = -5.0f;
    }

    return this->Position;
}
