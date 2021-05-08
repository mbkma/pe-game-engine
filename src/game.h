#ifndef GAME_H
#define GAME_H
#include <vector>
#include <tuple>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SFML/Audio.hpp>

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Represents the four possible directions
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

// Initial velocity of the Ball
const glm::vec3 INITIAL_BALL_VELOCITY(0.0f, 0.0f, 0.0f);

class Game
{
public:
    // game state
    GameState               State;
    bool                    Keys[1024];
    bool                    KeysProcessed[1024];
    unsigned int            Width, Height;

    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void Update(float dt);
    void Render();
    void RenderShadow();
    sf::Music music;

private:
    void FillPlayerList();
    void UpdateShaders();
};

#endif
