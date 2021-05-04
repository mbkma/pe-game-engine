#include "player-object.h"

#include "game.h"

PlayerObject::PlayerObject(Player *player,
                           Model *item,
                           btCollisionShape* pShape,
                           const float &mass,
                           const btVector3 &initialPosition,
                           const btQuaternion &initialRotation)
               :GameObject(item,
                           pShape,
                           mass,
                           initialPosition,
                           initialRotation)
{
    m_Player = player;
}
