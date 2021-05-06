#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <btBulletDynamicsCommon.h>

#include "model.h"
#include "shader.h"
#include "camera.h"


class Renderer
{
public:
    // Constructor (inits shaders/shapes)
    Renderer(Shader &shader,  Camera *camera);
    // Destructor
    ~Renderer();
    // Renders a defined model
    void Draw(Model *item, btScalar *transform = nullptr);
private:
    Shader       shader;
    Camera       *camera;
};

#endif
