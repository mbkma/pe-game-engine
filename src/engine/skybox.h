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
    Skybox(std::string path);

    Camera *m_pCamera;
    Shader *m_pSkyboxShader;

    void draw();

private:
    unsigned int loadCubemap(std::vector<std::string> faces);

    unsigned int cubemapTexture;

    unsigned int skyboxVAO, skyboxVBO;
};

#endif
