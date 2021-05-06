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
    Model       *Item;

    GameObject(){};
    GameObject(Model *item);
    ~GameObject() = default;

    // draw
    void Draw(Renderer &renderer, btScalar *transform = nullptr);
};

#endif
