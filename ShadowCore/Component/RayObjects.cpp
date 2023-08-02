#include "RayObjects.h"
#include "Core/Core.h"
using namespace SC;

AABB::AABB(std::shared_ptr<Object> _object) {
    component_type = RENDER_COMPONENT;
    if (_object->type == MESH || _object->type == RENDER_OBJECT) {
        box_debug_mesh = std::make_shared<RenderObject>("AABB", BuildAABB_Box(std::dynamic_pointer_cast<RenderObject>(_object)->geometry_data), USEFUL);
        CalculateMinMax(std::dynamic_pointer_cast<RenderObject>(_object)->transform.model, true);
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


std::shared_ptr<GeometryData> AABB::BuildAABB_Box(std::shared_ptr<GeometryData> _geom_data) {
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float minZ = std::numeric_limits<float>::infinity();
    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();
    float maxZ = -std::numeric_limits<float>::infinity();
    for (Vertex& v : *_geom_data->vertices) {
        glm::vec3 pos = v.position;
        if (pos.x < minX) minX = pos.x;
        if (pos.y < minY) minY = pos.y;
        if (pos.z < minZ) minZ = pos.z;
        if (pos.x > maxX) maxX = pos.x;
        if (pos.y > maxY) maxY = pos.y;
        if (pos.z > maxZ) maxZ = pos.z;
    }

    AABB::box.min = glm::vec3(minX, minY, minZ);
    AABB::box.max = glm::vec3(maxX, maxY, maxZ);

    AABB::start_box.min = glm::vec3(minX, minY, minZ);
    AABB::start_box.max = glm::vec3(maxX, maxY, maxZ);

    std::shared_ptr<std::vector<float>> aabbVertices = std::make_shared<std::vector<float>>(std::initializer_list<float>{
        box.min.x, box.min.y, box.min.z,
        box.max.x, box.min.y, box.min.z,
        box.max.x, box.max.y, box.min.z,
        box.min.x, box.max.y, box.min.z,
        box.min.x, box.min.y, box.max.z,
        box.max.x, box.min.y, box.max.z,
        box.max.x, box.max.y, box.max.z,
        box.min.x, box.max.y, box.max.z
    });

    std::shared_ptr<std::vector<Vertex>> res = RenderObject::ArrayToVertexPositionOnly(aabbVertices);
    std::shared_ptr<GeometryData> geom_data = std::make_shared<GeometryData>(res, aabbIndices);

    return geom_data;
}

AABB_Box AABB::CalculateMinMax(glm::mat4 model, bool rebuild = false) {
    AABB_Box res{};

    float a, b;

    res.min.x = res.max.x = model[3].x;
    res.min.y = res.max.y = model[3].y;
    res.min.z = res.max.z = model[3].z;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a = model[i][j] * start_box.min[j];
            b = model[i][j] * start_box.max[j];
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

    AABB::box.min = res.min;
    AABB::box.max = res.max;

    if (rebuild)
        AABB::ReBuildAABB_Box();

    return res;
}

void AABB::ReBuildAABB_Box() {
    box_debug_mesh->UnInitialize();

    std::shared_ptr<std::vector<float>> aabbVertices = std::make_shared<std::vector<float>>(std::initializer_list<float>{
       box.min.x, box.min.y, box.min.z,
       box.max.x, box.min.y, box.min.z,
       box.max.x, box.max.y, box.min.z,
       box.min.x, box.max.y, box.min.z,
       box.min.x, box.min.y, box.max.z,
       box.max.x, box.min.y, box.max.z,
       box.max.x, box.max.y, box.max.z,
       box.min.x, box.max.y, box.max.z
    });

    std::shared_ptr<std::vector<Vertex>> res = RenderObject::ArrayToVertexPositionOnly(aabbVertices);
    box_debug_mesh->geometry_data->vertices = res;
    box_debug_mesh->geometry_data->vertices_count = res->size();

    box_debug_mesh->Initialize(box_debug_mesh->geometry_data);
}