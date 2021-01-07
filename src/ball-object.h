#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game-object.h"

// BallObject holds the state of the Ball object inheriting
// relevant state data from GameObject.
class BallObject : public GameObject
{
public:
    float   Radius;

    // constructor(s)
    BallObject(glm::vec3 pos, Model item, float radius, glm::vec3 velocity);

    // moves the ball, returns new position
    glm::vec3 Move(float dt, unsigned int window_width);
};

#endif
