#ifndef ENGINE_H
#define ENGINE_H

#include "camera.h"
#include "debug-drawer.h"
#include "actor.h"
#include "physics.h"
/*#include "player-object.h"*/
#include "resource-manager.h"
#include "scene.h"
#include "scene-renderer.h"
#include "text-renderer.h"
#include "skybox.h"
#include "global.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <btBulletCollisionCommon.h>

class Engine
{
public:
    Engine();
    virtual ~Engine() = default;

    virtual void Init() = 0;

    virtual int Run();

    virtual void Update(float dt) = 0;

    virtual void Render() = 0;

    virtual void ProcessInput(float dt) = 0;

    virtual void ProcessMouseMovement(float xoffset, float yoffset, float xpos, float ypos) = 0;

    int GetScreenWidth();
    int GetScreenHeight();

    bool KeyPress(int key_id);
    bool KeyPressProcessed(int key_id);
    void SetKeyPressProcessed(int key_id, bool val);

private:

    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);


};

#endif
