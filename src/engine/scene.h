#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "debug-drawer.h"
#include "actor.h"
#include "physics.h"
/*#include "player-object.h"*/
/*#include "resource-manager.h"*/
/*#include "scene-renderer.h"*/
#include "text-renderer.h"
#include "skybox.h"

class SceneRenderer;

class Scene
{
public:

    Scene();
    ~Scene();

    std::map<std::string, Actor*> m_Actors{};
    Camera *m_pCamera{};
    Physics *m_pPhysic{};
    SceneRenderer *m_defaultRenderer{};
    TextRenderer *m_textRenderer{};
    DebugDrawer *m_pDebugDrawer{};
    Skybox *m_pSkybox{};

    bool m_shadow = false;

    void update(float dt);

    void addPhysic();


    void addSkybox(std::string path);
    Skybox* getSkybox() { return m_pSkybox; }

    void addActor(std::string name);
    Actor* getActor(std::string name);

    void addTextRenderer(unsigned int width, unsigned int height);
    TextRenderer* getTextRenderer() { return m_textRenderer; }

    void addDefaultRenderer(unsigned int width, unsigned int height);
    SceneRenderer* getDefaultRenderer() { return m_defaultRenderer; }

    void addDebugDrawer();
    DebugDrawer* getDebugDrawer() { return m_pDebugDrawer; }

    void addCamera(glm::vec3 position);
    Camera* getCamera() { return m_pCamera; }

    void render();

private:
    void updateActiveCamera();

};

#endif
