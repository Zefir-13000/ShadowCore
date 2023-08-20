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
        std::shared_ptr<RenderSequence> BuildAABB_Box(std::shared_ptr<RenderSequence> _obj_seq);
        void ReBuildAABB_Box();
        std::shared_ptr<AABB_Box> start_box = std::make_shared<AABB_Box>();
    public:
        std::shared_ptr<RenderObject> box_debug_mesh = nullptr;
        std::shared_ptr<AABB_Box> box = std::make_shared<AABB_Box>();

        std::shared_ptr<AABB_Box> CalculateMinMax(bool rebuild = false);
        void TraverseChildren(std::shared_ptr<Object> obj, std::shared_ptr<AABB_Box> parent);

        AABB(std::shared_ptr<Object> _object);

        void Render() {
            // Enable wireframe mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            // Draw the AABB
            box_debug_mesh->Render();

            // Disable wireframe mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        void Update() {
            CalculateMinMax(true);
        }
    };

};