#pragma once
#include "ssi.h"
#include "RenderObject.h"
#include "Component.h"

extern std::vector<unsigned int> aabbIndices;

namespace SC {

    struct AABB_Box {
        glm::vec3 min;
        glm::vec3 max;
    };

    class AABB : public RenderObject, public Component {
    private:
        std::vector<Vertex>& BuildAABB_Box(std::vector<Vertex>& _vertices);
        std::vector<Vertex>& BuildAABB_Box(std::vector<float>& _vertices);
        void ReBuildAABB_Box();
        AABB_Box start_box;
    public:

        AABB_Box box;

        AABB_Box CalculateMinMax(glm::mat4 model, bool rebuild);

        AABB(std::vector<Vertex>& _vertices) : RenderObject::RenderObject("AABB", BuildAABB_Box(_vertices), aabbIndices) {
            component_type = RENDER_COMPONENT;
        }
        AABB(std::vector<float>& _vertices) : RenderObject::RenderObject("AABB", BuildAABB_Box(_vertices), aabbIndices) {
            component_type = RENDER_COMPONENT;
        }

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

};