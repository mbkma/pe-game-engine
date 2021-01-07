#include "tournament.h"


Tournament::Tournament(std::string name, std::vector<Player> players)
: Name(name), Players(players)
{
    Init();
}

void Tournament::Init()
{
    // Init match plan

    // create new match
    Match *m = new Match(Players[0], Players[1]);

    // add match to list
    Matches.push_back(*m);
}

// should calculate all results of the next round
void Tournament::calculateNextRound()
{
    //TODO
}

// should return the Opponent for the next match
Player Tournament::getNextOpponent()
{
    //TODO
}
