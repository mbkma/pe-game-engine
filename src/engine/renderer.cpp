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

Renderer::Renderer(Shader *shader, Camera *camera)
{
    this->shader = shader;
    this->camera = camera;
}

Renderer::~Renderer()
{
    //TODO
}

void Renderer::Draw(Model *item, btScalar *transform)
{
    // prepare transformations
    this->shader->Use();

    glm::mat4 model = glm::mat4(1.0f);
    // model matrix
    if (transform != nullptr)
        model = btScalar2mat4(transform);

    this->shader->SetMatrix4("model", model);

//    std::cout<<glm::to_string(model)<<std::endl;

    if (item->getNumAnimations() > 0)
    {
        std::vector<glm::mat4> Transforms;
        item->boneTransform((float)glfwGetTime()*20, Transforms);
        this->shader->SetMatrix4v("gBones", Transforms);
    }

    item->Draw(this->shader);
}

