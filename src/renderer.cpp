#include "renderer.h"


Renderer::Renderer(Shader &shader, Camera *camera)
{
    this->shader = shader;
    this->camera = camera;
}

Renderer::~Renderer()
{
    //TODO
}

void Renderer::Draw(Model &item, glm::vec3 position, float size, float rotate)
{
    // prepare transformations
    this->shader.Use();

    // projection matrix is already set in game.cpp

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
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(size));
    this->shader.SetMatrix4("model", model);

    item.Draw(this->shader);
}

