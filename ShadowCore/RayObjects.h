#pragma once
#include "ssi.h"
#include "RenderObject.h"

extern std::vector<unsigned int> aabbIndices;

struct AABB_Box {
    glm::vec3 min;
    glm::vec3 max;
};

class AABB : public RenderObject {
private:
    std::vector<Vertex> BuildAABB_Box(std::vector<Vertex>& _vertices);
    std::vector<Vertex> BuildAABB_Box(std::vector<float>& _vertices);
    void ReBuildAABB_Box();
    
public:
    AABB_Box box;

    AABB_Box CalculateMinMax(glm::mat4 model, bool rebuild);

    AABB(std::vector<Vertex>& _vertices, glm::mat4 model) : RenderObject::RenderObject("AABB", BuildAABB_Box(_vertices), aabbIndices, AABB_BOX) {}
    AABB(std::vector<float>& _vertices, glm::mat4 model) : RenderObject::RenderObject("AABB", BuildAABB_Box(_vertices), aabbIndices, AABB_BOX) {}

    void Render() {
        //render_shader->Activate();
        //render_shader->setMat4("model", glm::mat4(1.0));
        //render_shader->setMat4("projection", enginePtr->level->main_cam->proj);
        //render_shader->setMat4("view", enginePtr->level->main_cam->view);
        // Enable wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Draw the AABB
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(36), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Disable wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
};

class Line : public RenderObject {
private:
    std::vector<Vertex> ConvertPosToLine(glm::vec3 _startPos, glm::vec3 _endPos);
public:
    Line(glm::vec3 _startPos, glm::vec3 _endPos) : RenderObject("Line", ConvertPosToLine(_startPos, _endPos), LINE) {}

    void Render() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
    }
};
