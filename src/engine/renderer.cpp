#include "renderer.h"

#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

glm::mat4 btScalar2mat4(btScalar* matrix) {
    return glm::mat4(
        matrix[0], matrix[1], matrix[2], matrix[3],
        matrix[4], matrix[5], matrix[6], matrix[7],
        matrix[8], matrix[9], matrix[10], matrix[11],
        matrix[12], matrix[13], matrix[14], matrix[15]);
}

Renderer::Renderer(Shader &shader, Camera *camera)
{
    this->shader = shader;
    this->camera = camera;
}

Renderer::~Renderer()
{
    //TODO
}

void Renderer::Draw(Model *item, btScalar *transform, float size)
{
    // prepare transformations
    this->shader.Use();

    this->shader.SetVector3f("viewPos", camera->Position);
    this->shader.SetFloat("material.shininess", 32.0f);

    this->shader.SetVector3f("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    this->shader.SetVector3f("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    this->shader.SetVector3f("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    this->shader.SetVector3f("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    this->shader.SetMatrix4("projection", projection);

    // view matrix
    glm::mat4 view = this->camera->GetViewMatrix();
    this->shader.SetMatrix4("view", view);

    // model matrix
    glm::mat4 model = btScalar2mat4(transform);
    model = glm::scale(model, glm::vec3(size));
    this->shader.SetMatrix4("model", model);

//    std::cout<<glm::to_string(model)<<std::endl;

    if (item->getNumAnimations() > 0)
    {
        std::vector<glm::mat4> Transforms;
        item->boneTransform((float)glfwGetTime(), Transforms);
        this->shader.SetMatrix4v("gBones", Transforms);
    }

    item->Draw(this->shader);
}

