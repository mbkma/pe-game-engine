#include "game-object.h"

//GameObject::GameObject()
//    : Position(0.0f, 0.0f), Size(1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Item() { }

GameObject::GameObject(glm::vec3 pos, Model item, float size, glm::vec3 velocity)
    : Position(pos), Item(item), Size(size), Velocity(velocity), Rotation(0.0f) { }

void GameObject::Draw(Renderer &renderer)
{
    renderer.Draw(this->Item, this->Position, this->Size, this->Rotation);
}
