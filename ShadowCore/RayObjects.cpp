#include "RayObjects.h"

std::vector<unsigned int> aabbIndices = {
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
};

std::vector<Vertex> AABB::BuildAABB_Box(std::vector<Vertex>& _vertices) {
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float minZ = std::numeric_limits<float>::infinity();
    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();
    float maxZ = -std::numeric_limits<float>::infinity();
    for (Vertex& v : _vertices) {
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

    std::vector<float> aabbVertices = {
        box.min.x, box.min.y, box.min.z,
        box.max.x, box.min.y, box.min.z,
        box.max.x, box.max.y, box.min.z,
        box.min.x, box.max.y, box.min.z,
        box.min.x, box.min.y, box.max.z,
        box.max.x, box.min.y, box.max.z,
        box.max.x, box.max.y, box.max.z,
        box.min.x, box.max.y, box.max.z
    };

    std::vector<Vertex> res = ArrayToVertexPositionOnly(aabbVertices);

    return res;
}

std::vector<Vertex> AABB::BuildAABB_Box(std::vector<float>& _vertices) {
    std::vector<Vertex> vertices = ArrayToVertex(_vertices);
    return AABB::BuildAABB_Box(vertices);
}

AABB_Box AABB::CalculateMinMax(glm::mat4 model, bool rebuild = false) {
    AABB_Box res{};

    float a, b;

    res.min.x = res.max.x = model[3].x;
    res.min.y = res.max.y = model[3].y;
    res.min.z = res.max.z = model[3].z;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a = model[i][j] * box.min[j];
            b = model[i][j] * box.max[j];
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
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    Inited = false;

    std::vector<float> aabbVertices = {
       box.min.x, box.min.y, box.min.z,
       box.max.x, box.min.y, box.min.z,
       box.max.x, box.max.y, box.min.z,
       box.min.x, box.max.y, box.min.z,
       box.min.x, box.min.y, box.max.z,
       box.max.x, box.min.y, box.max.z,
       box.max.x, box.max.y, box.max.z,
       box.min.x, box.max.y, box.max.z
    };
    std::cout << "min - X: " << box.min.x << " Y: " << box.min.y << " Z: " << box.min.z << " max - X: " << box.max.x << " Y: " << box.max.y << " Z: " << box.max.z << std::endl;

    std::vector<Vertex> res = ArrayToVertexPositionOnly(aabbVertices);

    Initialize(res, aabbIndices, AABB_BOX);
}

std::vector<Vertex> Line::ConvertPosToLine(glm::vec3 _startPos, glm::vec3 _endPos) {
    Vertex startLinePos{}, endLinePos{};
    startLinePos.position = _startPos;
    endLinePos.position = _endPos;
    std::vector<Vertex> res = { startLinePos, endLinePos };
    return res;
}
