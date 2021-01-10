#include "player-object.h"

#include "game.h"

PlayerObject::PlayerObject(Player player, glm::vec3 pos, Model item, float radius, glm::vec3 velocity)
    : GameObject(pos, item, radius, velocity), PlayerData(player), MovementSpeed(SPEED)
{

}

void PlayerObject::ProcessKeyboard(Direction direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == UP)
        this->Position.x += velocity;
    if (direction == DOWN)
        this->Position.x -= velocity;
    if (direction == LEFT)
        this->Position.z -= velocity;
    if (direction == RIGHT)
        this->Position.z += velocity;

//    Position.y = 0.0f;
}
