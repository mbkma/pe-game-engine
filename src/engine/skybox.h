#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "camera.h"
#include "shader.h"

class Skybox
{
public:
    Skybox();

    void Draw(Shader *shader, Camera *camera);

private:
    unsigned int loadCubemap(std::vector<std::string> faces);

    unsigned int cubemapTexture;

    unsigned int skyboxVAO, skyboxVBO;
};

#endif
