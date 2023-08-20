#include "RayObjects.h"
#include "Core/Core.h"
using namespace SC;

AABB::AABB(std::shared_ptr<Object> _object) : Component(_object) {
    component_type = RENDER_COMPONENT;
    if (_object->type == MESH || _object->type == RENDER_OBJECT) {
        box_debug_mesh = std::make_shared<RenderObject>("AABB", BuildAABB_Box(std::dynamic_pointer_cast<RenderObject>(_object)->renderSeq), USEFUL);
        CalculateMinMax(true);
    }
}

std::shared_ptr<std::vector<uint32_t>> aabbIndices = std::make_shared<std::vector<uint32_t>>(std::initializer_list<uint32_t>{
    0, 1, 2, // Front face
    2, 3, 0,
    1, 5, 6, // Right face
    6, 2, 1,
    7, 6, 5, // Back face
    5, 4, 7,
    4, 0, 3, // Left face
    3, 7, 4,
    4, 5, 1, // Bottom face
    1, 0, 4,
    3, 2, 6, // Top face
    6, 7, 3
});

void AABB::TraverseChildren(std::shared_ptr<Object> obj, std::shared_ptr<AABB_Box> parent) {
    if (obj->IsRenderAble()) {
        std::shared_ptr<RenderObject> Robj = std::dynamic_pointer_cast<RenderObject>(obj);
        for (std::shared_ptr<GeometryData> _geom_data : Robj->renderSeq->geoms_data) {
            if (_geom_data->render_type == NORENDER) {
                continue;
            }

            for (Vertex& v : *_geom_data->vertices) {
                glm::vec3 pos = v.position;
                if (pos.x < parent->min.x) parent->min.x = pos.x;
                if (pos.y < parent->min.y) parent->min.y = pos.y;
                if (pos.z < parent->min.z) parent->min.z = pos.z;
                if (pos.x > parent->max.x) parent->max.x = pos.x;
                if (pos.y > parent->max.y) parent->max.y = pos.y;
                if (pos.z > parent->max.z) parent->max.z = pos.z;
            }
        }

        for (std::shared_ptr<Object> child : Robj->transform->children) {
            if (!child->IsRenderAble()) {
                continue;
            }

            std::shared_ptr<RenderObject> childRObj = std::dynamic_pointer_cast<RenderObject>(child);
            for (std::shared_ptr<GeometryData> _geom_data : childRObj->renderSeq->geoms_data) {
                if (_geom_data->render_type == NORENDER) {
                    continue;
                }

                for (Vertex& v : *_geom_data->vertices) {
                    glm::vec3 pos = v.position;
                    if (pos.x < parent->min.x) parent->min.x = pos.x;
                    if (pos.y < parent->min.y) parent->min.y = pos.y;
                    if (pos.z < parent->min.z) parent->min.z = pos.z;
                    if (pos.x > parent->max.x) parent->max.x = pos.x;
                    if (pos.y > parent->max.y) parent->max.y = pos.y;
                    if (pos.z > parent->max.z) parent->max.z = pos.z;
                }
            }
            
            AABB::TraverseChildren(child, parent);
        }
    }
}

std::shared_ptr<RenderSequence> AABB::BuildAABB_Box(std::shared_ptr<RenderSequence> _obj_seq) {

    AABB::box->min = glm::vec3(std::numeric_limits<float>::infinity());
    AABB::box->max = glm::vec3(-std::numeric_limits<float>::infinity());

    AABB::TraverseChildren(Component::parent, AABB::box);

    AABB::start_box->min = glm::vec3(AABB::box->min.x, AABB::box->min.y, AABB::box->min.z);
    AABB::start_box->max = glm::vec3(AABB::box->max.x, AABB::box->max.y, AABB::box->max.z);

    std::shared_ptr<std::vector<float>> aabbVertices = std::make_shared<std::vector<float>>(std::initializer_list<float>{
        box->min.x, box->min.y, box->min.z,
        box->max.x, box->min.y, box->min.z,
        box->max.x, box->max.y, box->min.z,
        box->min.x, box->max.y, box->min.z,
        box->min.x, box->min.y, box->max.z,
        box->max.x, box->min.y, box->max.z,
        box->max.x, box->max.y, box->max.z,
        box->min.x, box->max.y, box->max.z
    });

    std::shared_ptr<std::vector<uint32_t>> indices = std::make_shared<std::vector<uint32_t>>(std::initializer_list<uint32_t>{
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    });

    std::shared_ptr<std::vector<Vertex>> res = RenderObject::ArrayToVertexPositionOnly(aabbVertices);
    std::shared_ptr<GeometryData> geom_data = std::make_shared<GeometryData>(res, indices, GL_LINES);
    std::shared_ptr<RenderSequence> seq = std::make_shared<RenderSequence>(std::initializer_list<std::shared_ptr<GeometryData>>({ geom_data }));

    return seq;
}

std::shared_ptr<AABB_Box> AABB::CalculateMinMax(bool rebuild) {
    AABB_Box res{};

    float a, b;

    glm::mat4 model = std::dynamic_pointer_cast<RenderObject>(parent)->transform->GetMatrix();

    res.min.x = res.max.x = model[3].x;
    res.min.y = res.max.y = model[3].y;
    res.min.z = res.max.z = model[3].z;

    model[2] = -model[2];

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a = model[i][j] * start_box->min[j];
            b = model[i][j] * start_box->max[j];
            if (a < b) {
                res.min[i] += a;
                res.max[i] += b;
            }
            else {
                res.min[i] += b;
                res.max[i] += a;
            }
        }
    }

    AABB::box->min = res.min;
    AABB::box->max = res.max;

    if (rebuild)
        AABB::ReBuildAABB_Box();

    return AABB::box;
}

void AABB::ReBuildAABB_Box() {
    std::shared_ptr<std::vector<float>> aabbVertices = std::make_shared<std::vector<float>>(std::initializer_list<float>{
       box->min.x, box->min.y, box->min.z,
       box->max.x, box->min.y, box->min.z,
       box->max.x, box->max.y, box->min.z,
       box->min.x, box->max.y, box->min.z,
       box->min.x, box->min.y, box->max.z,
       box->max.x, box->min.y, box->max.z,
       box->max.x, box->max.y, box->max.z,
       box->min.x, box->max.y, box->max.z
    });

    std::shared_ptr<std::vector<uint32_t>> indices = std::make_shared<std::vector<uint32_t>>(std::initializer_list<uint32_t>{
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    });

    std::shared_ptr<std::vector<Vertex>> res = RenderObject::ArrayToVertexPositionOnly(aabbVertices);
    std::shared_ptr<GeometryData> geom_data = std::make_shared<GeometryData>(res, indices, GL_LINES);
    std::shared_ptr<RenderSequence> seq = std::make_shared<RenderSequence>(std::initializer_list<std::shared_ptr<GeometryData>>({ geom_data }));

    box_debug_mesh->renderSeq = seq;
}