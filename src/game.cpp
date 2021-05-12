#include "game.h"

#include <algorithm>
#include <sstream>
#include <iostream>
#include <filesystem>

// Game-related State data
Scene *main_scene;
Scene *menu_scene;


btVector3 toBtVector3(glm::vec3 vec) {
    return btVector3(
        vec[0], vec[1], vec[2]);
}


void Game::Init()
{
    // load models
    ResourceManager::LoadModel(std::filesystem::path("../src/assets/models/stadium/stadium.obj").c_str(), true, "stadium");
    ResourceManager::LoadModel(std::filesystem::path("../src/assets/models/court/court.obj").c_str(), true, "court");
    ResourceManager::LoadModel(std::filesystem::path("../src/assets/models/ball/ball1.obj").c_str(), true, "ball");
    ResourceManager::LoadModel(std::filesystem::path("../src/assets/models/player/male-01-nude.dae").c_str(), true, "player");

    // music
    // all the music is freely available: http://www.manuchao.net/download-here-new-manu-chao-songs
//    music.openFromFile("../src/assets/sounds/fire-inna-streets.wav");
//    music.play();

    // configure game objects
    main_scene = new Scene();
    main_scene->addPhysic();
    main_scene->addDefaultRenderer(1600, 900);
    main_scene->addCamera(glm::vec3(0.0f, 2.0f, -1.0f));

    main_scene->addActor("stadium");
    main_scene->getActor("stadium")->AddRenderComponent(ResourceManager::GetModel("stadium"));


    main_scene->addActor("court");
    main_scene->getActor("court")->AddRenderComponent(ResourceManager::GetModel("court"));
//    main_scene->getActor("court")->AddPhysicComponent(new btBoxShape(btVector3(0.1f, 1.0f, 6.0f)), 0.0f);

    main_scene->addActor("ball");
    main_scene->getActor("ball")->AddPhysicComponent(new btSphereShape(0.05f), 1.0f);



//    main_scene->createPhysics(new btBoxShape(btVector3(15.0f, 25.05f, 15.0f)), 0, btVector3(0.0f, -25.0f, 0.0f));

    main_scene->addActor("player");
    main_scene->getActor("player")->AddRenderComponent(ResourceManager::GetModel("player"));
    main_scene->getActor("player")->AddPhysicControllerComponent();


    main_scene->addSkybox("../src/assets/models/skybox/");
//    main_scene->addDebugDrawer();


    menu_scene = new Scene();
    menu_scene->addTextRenderer(GetScreenWidth(), GetScreenHeight());
    menu_scene->getTextRenderer()->Load(std::filesystem::path("../src/assets/fonts/OCRAEXT.TTF").c_str(), 24);

    State = GAME_MENU;
}


void Game::Update(float dt)
{
    if (State == GAME_ACTIVE)
    {
        main_scene->update(dt);
    }
    if (State == GAME_MENU)
    {
    }
}

void Game::Render()
{
    if (State == GAME_ACTIVE)
    {

        main_scene->render();
    }

    if (State == GAME_MENU)
    {
        menu_scene->getTextRenderer()->RenderText("Press ENTER to start", 250.0f, 200.0f, 1.0f, glm::vec3(0.9f, 0.5f, 0.0f));
        menu_scene->getTextRenderer()->RenderText("Press ESC to exit", 245.0f, 220.0f, 0.75f);
    }

    if (State == GAME_WIN)
    {
    }
}

void Game::ProcessMouseMovement(float xoffset, float yoffset, float xpos, float ypos)
{
    if (State == GAME_ACTIVE)
    {
        main_scene->getCamera()->ProcessMouseMovement(xoffset, yoffset);
    }
    if (State == GAME_MENU)
    {
        //TODO
    }
    if (State == GAME_WIN)
    {
        //TODO
    }
}

void Game::ProcessInput(float dt)
{
    if (State == GAME_MENU)
    {
        if (KeyPress(GLFW_KEY_ENTER) && !KeyPressProcessed(GLFW_KEY_ENTER))
        {
            State = GAME_ACTIVE;
            SetKeyPressProcessed(GLFW_KEY_ENTER, true);
        }
    }
    if (State == GAME_WIN)
    {
        //TODO
    }
    if (State == GAME_ACTIVE)
    {
//        main_scene->getPlayerObject()->m_character->setWalkDirection(btVector3(0,0,0));


        if (KeyPress(GLFW_KEY_F) && !KeyPressProcessed(GLFW_KEY_F))
        {
            main_scene->m_shadow = !main_scene->m_shadow;
            SetKeyPressProcessed(GLFW_KEY_F, true);
        }

        if (KeyPress(GLFW_KEY_W))
        {
            main_scene->getActor("player")->ProcessKeyboard(UP, dt);
        }
        if (KeyPress(GLFW_KEY_S))
        {
            main_scene->getActor("player")->ProcessKeyboard(DOWN, dt);
        }
        if (KeyPress(GLFW_KEY_A))
        {
            main_scene->getActor("player")->ProcessKeyboard(LEFT, dt);
        }
        if (KeyPress(GLFW_KEY_D))
        {
            main_scene->getActor("player")->ProcessKeyboard(RIGHT, dt);
        }


        if (KeyPress(GLFW_KEY_G))
        {
            main_scene->getCamera()->ProcessKeyboard(LEFT, dt);
        }
        if (KeyPress(GLFW_KEY_J))
        {
            main_scene->getCamera()->ProcessKeyboard(RIGHT, dt);
        }
        if (KeyPress(GLFW_KEY_Y))
        {
            main_scene->getCamera()->ProcessKeyboard(UP, dt);
        }
        if (KeyPress(GLFW_KEY_H))
        {
            main_scene->getCamera()->ProcessKeyboard(DOWN, dt);
        }


        if (KeyPress(GLFW_KEY_R) && !KeyPressProcessed(GLFW_KEY_R))
        {
            // toggle wireframe debug drawing
            main_scene->getDebugDrawer()->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);

            SetKeyPressProcessed(GLFW_KEY_R, true);
        }
        if (KeyPress(GLFW_KEY_T) && !KeyPressProcessed(GLFW_KEY_T))
        {
            // toggle AABB debug drawing
            main_scene->getDebugDrawer()->ToggleDebugFlag(btIDebugDraw::DBG_DrawAabb);

            SetKeyPressProcessed(GLFW_KEY_T, true);
        }
    }
}


