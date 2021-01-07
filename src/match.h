#ifndef MATCH_H
#define MATCH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "player.h"

class Match
{
public:
    Player A;
    Player B;

    // constructor/destructor
    Match(Player a, Player b);
};

#endif
