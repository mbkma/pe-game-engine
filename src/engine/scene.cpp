#include "scene.h"

#include "scene-renderer.h"

Scene::Scene()
{
}

void Scene::update(float dt)
{
    // check if the world object exists
    if (m_pPhysic && m_pPhysic->m_pWorld)
    {
        // step the simulation through time
        m_pPhysic->m_pWorld->stepSimulation(dt);

        // check for any new collisions/separations
        m_pPhysic->CheckForCollisionEvents();
    }
}

void Scene::addPhysic()
{
    m_pPhysic = new Physics();
}

void addPlayerObject(std::string name, Model *item)
{
//    m_pPlayer = new PlayerObject(item);
}

void Scene::addActor(std::string name)
{
    m_Actors[name] = new Actor(name);
}

Actor* Scene::getActor(std::string name)
{
    return m_Actors[name];
}

void Scene::addCamera(glm::vec3 position)
{
    m_pCamera = new Camera(position);
    updateActiveCamera();
}

void Scene::addSkybox(std::string path)
{
    m_pSkybox = new Skybox(path);
}

void Scene::render()
{
    if(!m_defaultRenderer)
    {
        std::cout << "Error:Scene::render: m_defaultRenderer is not set!" << std::endl;
        return;
    }

    m_defaultRenderer->render(this);

    if (m_pDebugDrawer)
    {
        // after rendering all game objects, perform debug rendering
        // Bullet will figure out what needs to be drawn then call to
        // our DebugDrawer class to do the rendering for us
        m_pPhysic->m_pWorld->debugDrawWorld();
        m_pDebugDrawer->draw();
        m_pDebugDrawer->vertices.clear();
    }
}

void Scene::addTextRenderer(unsigned int width, unsigned int height)
{
    m_textRenderer = new TextRenderer(width, height);
}

void Scene::addDefaultRenderer(unsigned int width, unsigned int height)
{
    m_defaultRenderer = new SceneRenderer();
}

void Scene::addDebugDrawer()
{
    m_pDebugDrawer = new DebugDrawer();

    m_pDebugDrawer->setDebugMode(0);

    // add the debug drawer to the world
    m_pPhysic->m_pWorld->setDebugDrawer(m_pDebugDrawer);
}

void Scene::updateActiveCamera()
{
    if (m_defaultRenderer)
        m_defaultRenderer->m_pCamera = m_pCamera;
    if (m_pPhysic->m_pWorld)
        m_pPhysic->m_pCamera = m_pCamera;
    if (m_pDebugDrawer)
        m_pDebugDrawer->m_pCamera = m_pCamera;
    if (m_pSkybox)
        m_pSkybox->m_pCamera = m_pCamera;
}
