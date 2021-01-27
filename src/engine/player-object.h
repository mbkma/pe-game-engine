#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include <glm/glm.hpp>
#include <string>

#include "game-object.h"
#include "player.h"

class PlayerObject : public GameObject
{
public:
    Player *m_Player;

    // constructor/destructor
    PlayerObject(Player *player,
                 Model item,
                 btCollisionShape* pShape,
                 const float &mass,
                 const btVector3 &initialPosition,
                 const btQuaternion &initialRotation);
};

#endif
