#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include <glm/glm.hpp>
#include <string>

#include "game-object.h"
#include "player.h"

class PlayerObject : public GameObject
{
public:
    Player PlayerData;
    float MovementSpeed;

    // constructor/destructor
    PlayerObject(Player player, glm::vec3 pos, Model item, float radius = 1.0f, glm::vec3 velocity = glm::vec3(0,0,0));

    void ProcessKeyboard(Direction direction, float deltaTime);
};

#endif
