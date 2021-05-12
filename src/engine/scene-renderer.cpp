#include "scene-renderer.h"

#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "resource-manager.h"

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

SceneRenderer::SceneRenderer()
{
    m_pDefaultShader = ResourceManager::LoadShader("../src/shaders/default.vs", "../src/shaders/default.fs", nullptr, "default");

    m_projection = glm::perspective(glm::radians(FOV), SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
    m_lightPos = glm::vec3(1.0f, 3.0f, -2.0f);
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    glm::mat4 lightView = glm::lookAt(m_lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_lightSpaceMatrix = lightProjection * lightView;


        m_pDepthShader = ResourceManager::LoadShader("../src/shaders/shadow_mapping_depth.vs", "../src/shaders/shadow_mapping_depth.fs", nullptr, "simpleDepthShader");

        m_pDefaultShader->Use();
        m_pDefaultShader->SetInteger("shadowMap", 1);

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

SceneRenderer::~SceneRenderer()
{
    //TODO
}

void SceneRenderer::UpdateShaders()
{
m_pDefaultShader->Use();
 m_pDefaultShader->SetInteger("shadow", 0);
    m_pDefaultShader->SetFloat("material.shininess", 32.0f);
    m_pDefaultShader->SetVector3f("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    m_pDefaultShader->SetVector3f("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    m_pDefaultShader->SetVector3f("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    m_pDefaultShader->SetVector3f("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

    std::cout << glm::to_string(m_pCamera->Position) << std::endl;

    m_pDefaultShader->SetVector3f("viewPos", m_pCamera->Position);
    m_pDefaultShader->SetMatrix4("view", m_pCamera->GetViewMatrix());
    m_pDefaultShader->SetMatrix4("projection", m_projection);

    m_pDefaultShader->SetVector3f("lightPos", m_lightPos);
    m_pDefaultShader->SetMatrix4("lightSpaceMatrix", m_lightSpaceMatrix);
}


void SceneRenderer::drawShadow(Shader *shader, Scene *scene)
{
    glCullFace(GL_FRONT);

    // 1. render depth of scene to texture (from light's perspective)
    // --------------------------------------------------------------
    // render scene from light's point of view
    shader->Use();
    shader->SetMatrix4("lightSpaceMatrix", m_lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        drawAllGameObjects(shader, scene);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glCullFace(GL_BACK);

    // reset viewport
    glViewport(0, 0, 1600, 900);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void SceneRenderer::drawAllGameObjects(Shader *shader, Scene *scene)
{
    for (auto iter : scene->m_Actors)
    {
        iter.second->draw(shader);
    }
}

void SceneRenderer::render(Scene *scene)
{
    UpdateShaders();

    drawShadow(m_pDepthShader, scene);

    drawAllGameObjects(m_pDefaultShader, scene);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
}

