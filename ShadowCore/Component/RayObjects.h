#pragma once
#include "ssi.h"
#include "Object/RenderObject.h"
#include "Component/Component.h"

extern std::shared_ptr<std::vector<uint32_t>> aabbIndices;

namespace SC {

    struct AABB_Box {
        glm::vec3 min = glm::vec3(0.f);
        glm::vec3 max = glm::vec3(0.f);
    };

    class AABB : public Component {
    private:
        std::shared_ptr<GeometryData> BuildAABB_Box(std::shared_ptr<GeometryData> _geom_data);
        void ReBuildAABB_Box();
        AABB_Box start_box{};
    public:
        std::shared_ptr<RenderObject> box_debug_mesh = nullptr;
        AABB_Box box{};

        AABB_Box CalculateMinMax(glm::mat4 model, bool rebuild);

        AABB(std::shared_ptr<Object> _object);

        void Render() {
            // Enable wireframe mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            // Draw the AABB
            glBindVertexArray(box_debug_mesh->VAO);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(36), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Disable wireframe mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    };

};