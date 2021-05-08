#include "debug-drawer.h"



DebugDrawer::DebugDrawer()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

    glBindVertexArray(0);
}

void DebugDrawer::Draw(Shader *shader,  Camera *camera)
{
    shader->Use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1600.0f / 900.0f, 0.1f, 100.0f);
    shader->SetMatrix4("projection", projection);

    // view matrix
    glm::mat4 view = camera->GetViewMatrix();
    shader->SetMatrix4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    shader->SetMatrix4("model", model);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glDrawArrays(GL_LINES, 0, vertices.size());

    glBindVertexArray(0);
}

void DebugDrawer::insert(float x, float y, float z, float cx, float cy, float cz)
{
    Vertex vertex;

    vertex.Position.x = x;
    vertex.Position.y = y;
    vertex.Position.z = z;
    vertex.Color.x = cx;
    vertex.Color.y = cy;
    vertex.Color.z = cz;

    vertices.push_back(vertex);
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{
    insert(from.getX(), from.getY(), from.getZ(), fromColor.getX(), fromColor.getY(), fromColor.getZ());
    insert(to.getX(), to.getY(), to.getZ(), toColor.getX(), toColor.getY(), toColor.getZ());
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    drawLine(from, to, color, color);
}

void DebugDrawer::drawContactPoint(const btVector3 &pointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)
{
    // draws a line between two contact points
    btVector3 const startPoint = pointOnB;
    btVector3 const endPoint = pointOnB + normalOnB * distance;
    drawLine( startPoint, endPoint, color );
}

void DebugDrawer::ToggleDebugFlag(int flag) {
    // checks if a flag is set and enables/
    // disables it
    if (m_debugMode & flag) {
        // flag is enabled, so disable it
        m_debugMode = m_debugMode & (~flag);
    } else {
        // flag is disabled, so enable it
        m_debugMode |= flag;
    }
}
