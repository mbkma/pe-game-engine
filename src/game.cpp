#include <algorithm>
#include <sstream>
#include <iostream>
#include <filesystem>

#include <engine/camera.h>
#include <engine/resource-manager.h>
#include <engine/renderer.h>
#include <engine/game-object.h>
#include <engine/ball-object.h>
#include <engine/text-renderer.h>

#include "game.h"
#include "player.h"
#include "tournament.h"

using namespace std;

// Game-related State data
Renderer          *Rend;
Camera            *camera;
GameObject        *Box;
GameObject        *Court;
BallObject        *Ball;
TextRenderer      *Text;
std::vector<Player> Players;

std::ofstream file1;
std::ofstream file2;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), KeysProcessed(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Rend;
    delete Ball;
    delete Text;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("../src/shaders/default.vs", "../src/shaders/default.fs", nullptr, "default");

    // configure shaders
    // TODO

    // load models
    ResourceManager::LoadModel(filesystem::path("../src/models/court/court.obj").c_str(), true, "court");
    ResourceManager::LoadModel(filesystem::path("../src/models/ball/ball.obj").c_str(), true, "ball");
    ResourceManager::LoadModel(filesystem::path("../src/models/box/box.obj").c_str(), true, "box");

    // set camera
    camera = new Camera();

    // set render-specific controls
    Shader default_shader = ResourceManager::GetShader("default");
    Rend = new Renderer(default_shader, camera);
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load(filesystem::path("../src/fonts/OCRAEXT.TTF").c_str(), 24);

    // configure game objects
    glm::vec3 ballPos = glm::vec3(10.0f, 5.0f, 0.0f);
    glm::vec3 courtPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 boxPos = glm::vec3(-11.0f, 1.0f, 0.0f);
    Ball = new BallObject(ballPos, ResourceManager::GetModel("ball"), BALL_RADIUS, INITIAL_BALL_VELOCITY);
    Court = new GameObject(courtPos, ResourceManager::GetModel("court"));
    Box = new GameObject(boxPos, ResourceManager::GetModel("box"), 0.2f);
    file1.open("file1.dat");
    file2.open("file2.dat");

    // fill player list
    FillPlayerList();

    Tournament *t = new Tournament("ATP", Players);
}

void Game::Update(float dt)
{
    // update objects
    if (this->State == GAME_ACTIVE)
    {
        if (dt < 0.1f)
        {
            file1 <<  (float)glfwGetTime() << " " << Ball->Position.y << "\n";
            file2 <<  (float)glfwGetTime() << " " << Ball->Velocity.y << "\n";
            Ball->Move(dt, this->Width);
        }
    }
    if (this->State == GAME_MENU)
    {
        camera->Position.x = glm::cos((float)glfwGetTime()*0.1f) * 10.0f-10.0f;
        camera->Position.z = glm::sin((float)glfwGetTime()*0.1f) * 10.0f-20.0f;
    }

    // check for collisions
    //TODO
    //this->DoCollisions();

    // check loss condition
    //TODO
    // check win condition
    //TODO
}

void Game::ProcessMouseMovement(float xoffset, float yoffset)
{
    if (this->State == GAME_MENU)
    {
        //TODO
    }
    if (this->State == GAME_WIN)
    {
        //TODO
    }
    if (this->State == GAME_ACTIVE)
    {
        camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_MENU)
    {
        if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
        {
            this->State = GAME_ACTIVE;
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
            camera->Yaw = 0.0f;
            camera->Position = glm::vec3(-20.0f, 6.0f, 0.0f);
            camera->updateCameraVectors();
        }
    }
    if (this->State == GAME_WIN)
    {
        //TODO
    }
    if (this->State == GAME_ACTIVE)
    {
        if (this->Keys[GLFW_KEY_W])
            camera->ProcessKeyboard(UP, dt);
        if (this->Keys[GLFW_KEY_S])
            camera->ProcessKeyboard(DOWN, dt);
        if (this->Keys[GLFW_KEY_A])
            camera->ProcessKeyboard(LEFT, dt);
        if (this->Keys[GLFW_KEY_D])
            camera->ProcessKeyboard(RIGHT, dt);
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        Ball->Draw(*Rend);
        Court->Draw(*Rend);
        Box->Draw(*Rend);
    }

    if (this->State == GAME_MENU)
    {
        Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2.0f, 1.0f, glm::vec3(0.9f, 0.5f, 0.0f));
        Text->RenderText("Press ESC to exit", 245.0f, this->Height / 2.0f + 20.0f, 0.75f);
        Court->Draw(*Rend);
    }

    if (this->State == GAME_WIN)
    {
        Text->RenderText("You WON!!!", 320.0f, this->Height / 2.0f - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->Height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    }
}

void Game::FillPlayerList()
{
    // create new player
    Player *p = new Player("Nadal", 40, 180, 1, false, 99);
    // insert player in list
    Players.push_back(*p);

    p = new Player("Federer", 50, 190, 2, true, 90);
    Players.push_back(*p);
}

// collision detection
//bool CheckCollision(GameObject &one, GameObject &two);
//Collision CheckCollision(BallObject &one, GameObject &two);

//void Game::DoCollisions()
//{
//        //TODO
//}

//bool CheckCollision(GameObject &one, GameObject &two)
//{
//        //TODO
//}

//Collision CheckCollision(BallObject &one, GameObject &two)
//{
//        //TODO
//}
