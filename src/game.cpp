#include <algorithm>
#include <sstream>
#include <iostream>
#include <filesystem>

#include <engine/camera.h>
#include <engine/physics.h>
#include <engine/physic-object.h>
#include <engine/player-object.h>
#include <engine/game-object.h>
#include <engine/resource-manager.h>
#include <engine/renderer.h>
#include <engine/ball-object.h>
#include <engine/text-renderer.h>
#include <engine/debug-drawer.h>
#include <engine/skybox.h>

#include <btBulletCollisionCommon.h>

#include "game.h"

#include "player.h"
#include "tournament.h"

using namespace std;

// Game-related State data
Physics           *Physic;
Renderer          *DefaultRenderer;
Renderer          *AnimationRenderer;
Renderer          *ShadowRenderer;
Camera            *camera;
PlayerObject        *PlayerO;
GameObject        *Court;
GameObject        *Stadium;
PhysicObject        *Ball;
PhysicObject        *Ground;
PhysicObject        *Test;
Skybox              *skybox;
TextRenderer      *Text;
DebugDrawer* m_pDebugDrawer;
std::vector<Player> Players;
Shader *default_shader;
Shader *animation_shader;
Shader *color_shader;
Shader *skybox_shader;
Shader *simpleDepthShader;

Renderer          *renderer;

unsigned int depthMap;
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depthMapFBO;

// The Width of the screen
const float SCREEN_WIDTH = 1600;
// The height of the screen
const float SCREEN_HEIGHT = 900;


const float FAR_PLANE = 100.0f;
const float NEAR_PLANE = 0.1f;

const float FOV = 45.0f;

btVector3 toBtVector3(glm::vec3 vec) {
    return btVector3(
        vec[0], vec[1], vec[2]);
}


Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU),
      Keys(),
      KeysProcessed(),
      Width(width),
      Height(height)
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
    ResourceManager::LoadShader("../src/shaders/colors.vs", "../src/shaders/colors.fs", nullptr, "color");
    ResourceManager::LoadShader("../src/shaders/skybox.vs", "../src/shaders/skybox.fs", nullptr, "skybox");
    ResourceManager::LoadShader("../src/shaders/shadow_mapping_depth.vs", "../src/shaders/shadow_mapping_depth.fs", nullptr, "simpleDepthShader");
    // configure shaders
    // TODO

    // load models
    ResourceManager::LoadModel(filesystem::path("../src/models/stadium/stadium.obj").c_str(), true, "stadium");
    ResourceManager::LoadModel(filesystem::path("../src/models/court/court.obj").c_str(), true, "court");
    ResourceManager::LoadModel(filesystem::path("../src/models/ball/ball.obj").c_str(), true, "ball");
    ResourceManager::LoadModel(filesystem::path("../src/models/box/box.obj").c_str(), true, "player");

    // set camera
    camera = new Camera(glm::vec3(0.0f, 2.0f, 0.0f));

    // set render-specific controls
    default_shader = ResourceManager::GetShader("default");
    animation_shader = ResourceManager::GetShader("animation");
    color_shader = ResourceManager::GetShader("color");
    skybox_shader = ResourceManager::GetShader("skybox");
    simpleDepthShader = ResourceManager::GetShader("simpleDepthShader");
    DefaultRenderer = new Renderer(default_shader, camera);
    AnimationRenderer = new Renderer(animation_shader, camera);
    ShadowRenderer = new Renderer(simpleDepthShader, camera);
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load(filesystem::path("../src/fonts/OCRAEXT.TTF").c_str(), 24);


    default_shader->Use();
    default_shader->SetInteger("shadowMap", 1);

    // music
    // all the music is freely available: http://www.manuchao.net/download-here-new-manu-chao-songs
//    music.openFromFile("../src/sounds/fire-inna-streets.wav");
//    music.play();

    skybox = new Skybox();

    Physic = new Physics(camera);

    // configure game objects
    Stadium = new GameObject(ResourceManager::GetModel("stadium"));

    Court = new GameObject(ResourceManager::GetModel("court"));

    Ball = new PhysicObject(ResourceManager::GetModel("ball"),
                            new btSphereShape(3.0f),
                            1.0f,
                            btVector3(-5.0f, 5.0f, 0.0f));

    PlayerO = new PlayerObject(ResourceManager::GetModel("player"),
                               btVector3(-10.0f, 0.0f, -3.0f),
                               btQuaternion(1,0,0,1),
                               nullptr);


    Ground = new PhysicObject(nullptr,
                            new btBoxShape(btVector3(15.0f, 25.05f, 15.0f)),
                            0,
                            btVector3(0.0f, -25.0f, 0.0f));

    Test = new PhysicObject(nullptr,
                            new btBoxShape(btVector3(0.1f, 1.0f, 6.0f)),
                            0,
                            btVector3(0.0f, 0.0f, 0.0f));

    ResourceManager::PhysicObjects["Ball"] = Ball;
    ResourceManager::PhysicObjects["Player"] = PlayerO;
    ResourceManager::PhysicObjects["Ground"] = Ground;


    // check if the world object is valid
    if (Physic->m_pWorld) {
        // add the object's rigid body to the world
        Physic->m_pWorld->addRigidBody(Ball->GetRigidBody());
        Physic->m_pWorld->addRigidBody(Ground->GetRigidBody());
        Physic->m_pWorld->addRigidBody(Test->GetRigidBody());

        Physic->m_pBroadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
        Physic->m_pWorld->addCollisionObject(PlayerO->m_ghostObject,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);

        Physic->m_pWorld->addAction(PlayerO->m_character);
    }

    // create the debug drawer
    m_pDebugDrawer = new DebugDrawer();
    // set the initial debug level to 0
    m_pDebugDrawer->setDebugMode(0);

    // add the debug drawer to the world
    Physic->m_pWorld->setDebugDrawer(m_pDebugDrawer);

    // fill player list
    FillPlayerList();

    std::cout << "numAnim: " << PlayerO->Item->getNumAnimations() << std::endl;

//    Tournament *t = new Tournament("ATP", Players);

    // INIT SHADOW


    // configure depth map FBO
    // -----------------------

    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
        camera->Position.x = glm::cos((float)glfwGetTime()*0.1f) * 10.0f-40.0f;
        camera->Position.z = glm::sin((float)glfwGetTime()*0.1f) * 10.0f-40.0f;
        camera->Position.y = 30.0f;
    }
}

void Game::ProcessMouseMovement(float xoffset, float yoffset, float xpos, float ypos)
{
    if (this->State == GAME_ACTIVE)
    {
//        camera->ProcessMouseMovement(xoffset, yoffset);


        Physics::RayResult result;
        btVector3 direction = Physic->GetPickingRay(xpos, ypos);
        // perform the raycast
        if (!Physic->Raycast(toBtVector3(camera->Position), direction, result))
            return; // return if the test failed
        Physic->Shoot(btVector3(0,1,0), result.pBody);
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
        PlayerO->m_character->setWalkDirection(btVector3(0,0,0));

        if (this->Keys[GLFW_KEY_W])
        {
            PlayerO->ProcessKeyboard(UP, dt);
        }
        if (this->Keys[GLFW_KEY_S])
        {
            PlayerO->ProcessKeyboard(DOWN, dt);
        }
        if (this->Keys[GLFW_KEY_A])
        {
            PlayerO->ProcessKeyboard(LEFT, dt);
        }
        if (this->Keys[GLFW_KEY_D])
        {
            PlayerO->ProcessKeyboard(RIGHT, dt);
        }


        if (this->Keys[GLFW_KEY_G])
        {
            camera->ProcessKeyboard(LEFT, dt);
        }
        if (this->Keys[GLFW_KEY_J])
        {
            camera->ProcessKeyboard(RIGHT, dt);
        }
        if (this->Keys[GLFW_KEY_Y])
        {
            camera->ProcessKeyboard(UP, dt);
        }
        if (this->Keys[GLFW_KEY_H])
        {
            camera->ProcessKeyboard(DOWN, dt);
        }


        if (this->Keys[GLFW_KEY_R])
        {
            // toggle wireframe debug drawing
            m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);
        }
        if (this->Keys[GLFW_KEY_T])
        {
            // toggle AABB debug drawing
            m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawAabb);
        }
    }
}

void Game::UpdateShaders()
{
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(FOV), SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
    glm::vec3 lightPos(1.0f, 3.0f, -2.0f);
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    default_shader->Use();
    default_shader->SetFloat("material.shininess", 32.0f);
    default_shader->SetVector3f("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    default_shader->SetVector3f("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    default_shader->SetVector3f("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    default_shader->SetVector3f("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

    default_shader->SetVector3f("viewPos", camera->Position);
    default_shader->SetMatrix4("view", view);
    default_shader->SetMatrix4("projection", projection);

    default_shader->SetVector3f("lightPos", lightPos);
    default_shader->SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);

    animation_shader->Use();
    animation_shader->SetMatrix4("view", view);
    animation_shader->SetMatrix4("projection", projection);
}

void Game::RenderShadow()
{
    glCullFace(GL_FRONT);

    glm::vec3 lightPos(1.0f, 3.0f, -2.0f);
    // 1. render depth of scene to texture (from light's perspective)
    // --------------------------------------------------------------
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    // render scene from light's point of view
    simpleDepthShader->Use();
    simpleDepthShader->SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        renderer = ShadowRenderer;
        Render();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glCullFace(GL_BACK);

    // reset viewport
    glViewport(0, 0, 1600, 900);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Game::Render()
{
    btScalar transform[16];

    UpdateShaders();

    if (this->State == GAME_ACTIVE)
    {
        Court->Draw(*renderer);
        Stadium->Draw(*renderer);
        Ball->GetTransform(transform);
        Ball->Draw(*renderer, transform);

        PlayerO->GetTransform(transform);
        PlayerO->Draw(*renderer, transform);

        // after rendering all game objects, perform debug rendering
        // Bullet will figure out what needs to be drawn then call to
        // our DebugDrawer class to do the rendering for us
        Physic->m_pWorld->debugDrawWorld();
        m_pDebugDrawer->Draw(color_shader, camera);
        m_pDebugDrawer->vertices.clear();

        skybox->Draw(skybox_shader, camera);
    }

    if (this->State == GAME_MENU)
    {
        Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2.0f, 1.0f, glm::vec3(0.9f, 0.5f, 0.0f));
        Text->RenderText("Press ESC to exit", 245.0f, this->Height / 2.0f + 20.0f, 0.75f);

        Court->Draw(*renderer);
        Stadium->Draw(*renderer);
        skybox->Draw(skybox_shader, camera);

        camera->Yaw = 60.0f;
        camera->Pitch = -20.0f;
        camera->updateCameraVectors();
    }

    if (this->State == GAME_WIN)
    {
        Text->RenderText("You WON!!!", 320.0f, this->Height / 2.0f - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->Height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    }

    renderer = DefaultRenderer;

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
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

