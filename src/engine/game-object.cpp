#include "game-object.h"

GameObject::GameObject(Model *item)
                      :Item(item)
{
}

void GameObject::Draw(Renderer &renderer, btScalar *transform)
{
    renderer.Draw(Item);
}
