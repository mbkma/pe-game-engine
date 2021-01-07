#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "player.h"
#include "match.h"

class Tournament
{
public:
    std::string Name;
    std::vector<Player> Players;
    std::vector<Match> Matches;

    // constructor/destructor
    Tournament(std::string name, std::vector<Player> players);
    ~Tournament();

    void Init();
    Player getNextOpponent();
private:
    void calculateNextRound();
};

#endif
