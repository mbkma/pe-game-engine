#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    void Draw(Model &item, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float size = 1.0f, float rotate = 0.0f);
private:
    Shader       shader;
    Camera       *camera;
};

#endif
