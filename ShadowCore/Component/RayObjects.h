#pragma once
#include "ssi.h"
#include "Object/RenderObject.h"
#include "Component/Component.h"

extern std::shared_ptr<std::vector<uint32_t>> aabbIndices;

namespace SC {

    struct AABB_Box {
        glm::vec3 min;
        glm::vec3 max;
    };

    class AABB : public Component {
    private:
        std::shared_ptr<std::vector<Vertex>> BuildAABB_Box(std::shared_ptr<std::vector<Vertex>> _vertices);
        std::shared_ptr<std::vector<Vertex>> BuildAABB_Box(std::shared_ptr<std::vector<float>> _vertices);
        void ReBuildAABB_Box();
        AABB_Box start_box;
    public:
        std::shared_ptr<RenderObject> box_debug_mesh = nullptr;
        AABB_Box box;

        AABB_Box CalculateMinMax(glm::mat4 model, bool rebuild);

        AABB(std::shared_ptr<std::vector<Vertex>> _vertices) {
            component_type = RENDER_COMPONENT;
            box_debug_mesh = std::make_shared<RenderObject>("AABB", BuildAABB_Box(_vertices), aabbIndices);
        }
        AABB(std::shared_ptr<std::vector<float>> _vertices) {
            component_type = RENDER_COMPONENT;
            box_debug_mesh = std::make_shared<RenderObject>("AABB", BuildAABB_Box(_vertices), aabbIndices);
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
            glBindVertexArray(box_debug_mesh->VAO);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(36), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Disable wireframe mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    };

};