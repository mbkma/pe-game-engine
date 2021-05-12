#ifndef SCENE_RENDERER_H
#define SCENE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model.h"
#include "shader.h"
#include "scene.h"
#include "camera.h"

class SceneRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SceneRenderer();

    // Destructor
    ~SceneRenderer();

    Camera *m_pCamera;

    void UpdateShaders();

    void render(Scene *scene);


private:
    Shader *m_pDefaultShader;
    Shader *m_pDepthShader;

    glm::vec3 m_lightPos;
    glm::mat4 m_projection;
    glm::mat4 m_lightSpaceMatrix;

    void drawShadow(Shader *shader, Scene *scene);
    void drawAllGameObjects(Shader *shader, Scene *scene);
};

#endif
