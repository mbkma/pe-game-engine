#ifndef DEBUG_DRAWER_H
#define DEBUG_DRAWER_H

#include <vector>

#include <glad/glad.h>
#include <LinearMath/btIDebugDraw.h>

#include "shader.h"
#include "camera.h"

class DebugDrawer : public btIDebugDraw {
public:
    DebugDrawer();

    // debug mode functions
    virtual void setDebugMode(int debugMode) override { m_debugMode = debugMode; }
    virtual int getDebugMode() const override { return m_debugMode; }

    // drawing functions
    virtual void  drawContactPoint(const btVector3 &pointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) override;
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

    // unused
    virtual void  reportErrorWarning(const char* warningString) override {}
    virtual void  draw3dText(const btVector3 &location,const char* textString) override {}

    void ToggleDebugFlag(int flag);

    unsigned int VAO;

	unsigned i = 0;

    void Draw(Shader &shader,  Camera *camera);

    struct Vertex {
        // position
        glm::vec3 Position;
        // color
        glm::vec3 Color;
    };

    std::vector<Vertex> vertices;

protected:
    // render data
    unsigned int VBO, EBO;

    int m_debugMode;

    void insert(float x, float y, float z, float cx, float cy, float cz);
};


#endif // DEBUG_DRAWER_H
