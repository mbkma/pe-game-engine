#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <string>

class Player
{
public:
    // general
    std::string Name;
    int Age;
    int Height;
    int Ranking;
    bool RightHanded;
    int Ability;

    // constructor/destructor
    Player(std::string name, int age, int height, int ranking, bool right_handed, int ability);

    void scored();
};

#endif
