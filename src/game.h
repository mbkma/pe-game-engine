#ifndef GAME_H
#define GAME_H

#include <SFML/Audio.hpp>

#include <engine/engine.h>

class Game : public Engine
{
public:
    // game state
    GameState               State;
    // music
    sf::Music music;

    // constructor/destructor
    Game(){};
    virtual ~Game() = default;

    // initialize game state (load all shaders/textures/levels)

    virtual void ProcessInput(float dt) override;
    virtual void ProcessMouseMovement(float xoffset, float yoffset, float xpos, float ypos) override;
    virtual void Init() override;
    virtual void Update(float dt) override;
    virtual void Render() override;
};

#endif
