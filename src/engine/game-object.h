#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "renderer.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    glm::vec3   Position;
    Model       Item;
    float       Size;
    glm::vec3   Velocity;
    float       Rotation;

    // constructor(s)
/*    GameObject();*/
    GameObject(glm::vec3 pos, Model item, float size = 1.0f, glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f), float rotation = 0.0f);
    virtual ~GameObject() = default;

    // draw
    virtual void Draw(Renderer &renderer);
};

#endif
