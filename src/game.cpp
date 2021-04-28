#include <algorithm>
#include <sstream>
#include <iostream>
#include <filesystem>

#include <engine/camera.h>
#include <engine/resource-manager.h>
#include <engine/renderer.h>
#include <engine/game-object.h>
#include <engine/physics.h>
#include <engine/player-object.h>
#include <engine/ball-object.h>
#include <engine/text-renderer.h>

#include <btBulletCollisionCommon.h>

#include "game.h"
#include "player.h"
#include "tournament.h"

using namespace std;

// Game-related State data
Physics           *Physic;
Renderer          *DefaultRenderer;
Renderer          *AnimationRenderer;
Camera            *camera;
GameObject        *PlayerO;
GameObject        *Court;
GameObject        *Stadium;
GameObject        *Ball;
TextRenderer      *Text;
std::vector<Player> Players;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), KeysProcessed(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete DefaultRenderer;
    delete AnimationRenderer;
    delete camera;
    delete PlayerO;
    delete Court;
    delete Stadium;
    delete Ball;
    delete Text;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("../src/shaders/default.vs", "../src/shaders/default.fs", nullptr, "default");
    ResourceManager::LoadShader("../src/shaders/animation.vs", "../src/shaders/default.fs", nullptr, "animation");

    // configure shaders
    // TODO

    // load models
    ResourceManager::LoadModel(filesystem::path("../src/models/stadium/stadium.obj").c_str(), true, "stadium");
    ResourceManager::LoadModel(filesystem::path("../src/models/court/court.obj").c_str(), true, "court");
    ResourceManager::LoadModel(filesystem::path("../src/models/ball/ball.obj").c_str(), true, "ball");
    ResourceManager::LoadModel(filesystem::path("../src/models/player/player.dae").c_str(), true, "player");

    // set camera
    camera = new Camera(glm::vec3(0.0f, 2.0f, 0.0f));

    // set render-specific controls
    Shader default_shader = ResourceManager::GetShader("default");
    Shader animation_shader = ResourceManager::GetShader("animation");
    DefaultRenderer = new Renderer(default_shader, camera);
    AnimationRenderer = new Renderer(animation_shader, camera);
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load(filesystem::path("../src/fonts/OCRAEXT.TTF").c_str(), 24);

    // music
    // all the music is freely available: http://www.manuchao.net/download-here-new-manu-chao-songs
    music.openFromFile("../src/sounds/fire-inna-streets.wav");
    music.play();

    // configure game objects
    Stadium = new GameObject(*ResourceManager::GetModel("stadium"),
                             new btBoxShape(btVector3(50,0,50)),
                             0.0,
                             btVector3(0.0f, 0.0f, 0.0f),
                             btQuaternion(0,0,0,1));

    Court = new GameObject(*ResourceManager::GetModel("court"),
                           new btBoxShape(btVector3(50,0.03f,50)),
                           0.0,
                           btVector3(0.0f, 0.0f, 0.0f),
                           btQuaternion(0,0,0,1));

    Ball = new GameObject(*ResourceManager::GetModel("ball"),
                          new btSphereShape(0.06f),
                          1.0f,
                          btVector3(0.0f, 5.0f, 0.0f));


    PlayerO = new GameObject(*ResourceManager::GetModel("player"),
                             new btSphereShape(1.0f),
                             1.0f,
                             btVector3(0.0f, 1.0f, 0.0f));

    ResourceManager::GameObjects["Stadium"] = Stadium;
    ResourceManager::GameObjects["Court"] = Court;
    ResourceManager::GameObjects["Ball"] = Ball;
    ResourceManager::GameObjects["Player"] = PlayerO;

    Physic = new Physics();

    // check if the world object is valid
    if (Physic->m_pWorld) {
        // add the object's rigid body to the world
        Physic->m_pWorld->addRigidBody(Ball->GetRigidBody());
        Physic->m_pWorld->addRigidBody(Court->GetRigidBody());
        Physic->m_pWorld->addRigidBody(PlayerO->GetRigidBody());
    }

    // fill player list
    FillPlayerList();

//    Tournament *t = new Tournament("ATP", Players);
}

void Game::Update(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        // check if the world object exists
        if (Physic->m_pWorld) {
            // step the simulation through time
            Physic->m_pWorld->stepSimulation(dt);

            // check for any new collisions/separations
            Physic->CheckForCollisionEvents();
        }
    }
    if (this->State == GAME_MENU)
    {
        camera->Position.x = glm::cos((float)glfwGetTime()*0.1f) * 10.0f-10.0f;
        camera->Position.z = glm::sin((float)glfwGetTime()*0.1f) * 10.0f-20.0f;
        camera->Position.y = 30.0f;
    }
}

void Game::ProcessMouseMovement(float xoffset, float yoffset)
{
    if (this->State == GAME_ACTIVE)
    {
        camera->ProcessMouseMovement(xoffset, yoffset);
    }
    if (this->State == GAME_MENU)
    {
        //TODO
    }
    if (this->State == GAME_WIN)
    {
        //TODO
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
    btScalar transform[16];

    if (this->State == GAME_ACTIVE)
    {
        Ball->GetTransform(transform);
        Ball->Draw(*DefaultRenderer, transform);

        Court->GetTransform(transform);
        Court->Draw(*DefaultRenderer, transform);

        Stadium->GetTransform(transform);
        Stadium->Draw(*DefaultRenderer, transform);

        PlayerO->GetTransform(transform);
        PlayerO->Draw(*AnimationRenderer, transform);
    }

    if (this->State == GAME_MENU)
    {
        Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2.0f, 1.0f, glm::vec3(0.9f, 0.5f, 0.0f));
        Text->RenderText("Press ESC to exit", 245.0f, this->Height / 2.0f + 20.0f, 0.75f);
        Court->GetTransform(transform);
        Court->Draw(*DefaultRenderer, transform);
        Stadium->GetTransform(transform);
        Stadium->Draw(*DefaultRenderer, transform);
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

