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

std::vector<Vertex> AABB::BuildAABB_Box(glm::vec3 _min, glm::vec3 _max) {
    std::vector<float> aabbVertices = {
        _min.x, _min.y, _min.z,
        _max.x, _min.y, _min.z,
        _max.x, _max.y, _min.z,
        _min.x, _max.y, _min.z,
        _min.x, _min.y, _max.z,
        _max.x, _min.y, _max.z,
        _max.x, _max.y, _max.z,
        _min.x, _max.y, _max.z
    };

    std::vector<Vertex> res = ArrayToVertexPositionOnly(aabbVertices);

    return res;
}

std::vector<Vertex> AABB::CalculateMinMax(std::vector<Vertex>& _vertices, bool rebuild = false) {
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float minZ = std::numeric_limits<float>::infinity();
    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();
    float maxZ = -std::numeric_limits<float>::infinity();
    for (Vertex& v : _vertices) {
        if (v.position.x < minX) minX = v.position.x;
        if (v.position.y < minY) minY = v.position.y;
        if (v.position.z < minZ) minZ = v.position.z;
        if (v.position.x > maxX) maxX = v.position.x;
        if (v.position.y > maxY) maxY = v.position.y;
        if (v.position.z > maxZ) maxZ = v.position.z;
    }

    AABB::box.min = glm::vec3(minX, minY, minZ);
    AABB::box.max = glm::vec3(maxX, maxY, maxZ);

    std::vector<Vertex> vertices = AABB::BuildAABB_Box(AABB::box.min, AABB::box.max);

    if (rebuild)
        AABB::ReBuildAABB_Box(vertices);

    return vertices;
}

std::vector<Vertex> AABB::CalculateMinMax(std::vector<float>& _vertices, bool rebuild) {
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float minZ = std::numeric_limits<float>::infinity();
    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();
    float maxZ = -std::numeric_limits<float>::infinity();

    std::vector<Vertex> _vvertices = ArrayToVertex(_vertices);

    for (Vertex& v : _vvertices) {
        if (v.position.x < minX) minX = v.position.x;
        if (v.position.y < minY) minY = v.position.y;
        if (v.position.z < minZ) minZ = v.position.z;
        if (v.position.x > maxX) maxX = v.position.x;
        if (v.position.y > maxY) maxY = v.position.y;
        if (v.position.z > maxZ) maxZ = v.position.z;
    }

    AABB::box.min = glm::vec3(minX, minY, minZ);
    AABB::box.max = glm::vec3(maxX, maxY, maxZ);

    std::vector<Vertex> vertices = AABB::BuildAABB_Box(AABB::box.min, AABB::box.max);

    if (rebuild)
        AABB::ReBuildAABB_Box(vertices);

    return vertices;
}

void AABB::ReBuildAABB_Box(std::vector<Vertex>& _vertices) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    RenderObject("AABB", _vertices, aabbIndices, AABB_BOX);
}

std::vector<Vertex> Line::ConvertPosToLine(glm::vec3 _startPos, glm::vec3 _endPos) {
    Vertex startLinePos{}, endLinePos{};
    startLinePos.position = _startPos;
    endLinePos.position = _endPos;
    std::vector<Vertex> res = { startLinePos, endLinePos };
    return res;
}
