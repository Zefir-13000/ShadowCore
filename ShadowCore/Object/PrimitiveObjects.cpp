#include "PrimitiveObjects.h"

using namespace SC;
std::shared_ptr<std::vector<uint32_t>> SC::CubeIndices = std::make_shared<std::vector<uint32_t>>(std::initializer_list<uint32_t>{
        0, 1, 2, 2, 3, 0, // Front face
        1, 5, 6, 6, 2, 1, // Right face
        7, 6, 5, 5, 4, 7, // Back face
        4, 0, 3, 3, 7, 4, // Left face
        4, 5, 1, 1, 0, 4, // Bottom face
        3, 2, 6, 6, 7, 3  // Top face
});

std::shared_ptr<GeometryData> Line::ConvertPosToLine(glm::vec3 _startPos, glm::vec3 _endPos) {
    Vertex startLinePos{ .position = _startPos }, endLinePos{ .position = _endPos };
    std::shared_ptr<std::vector<Vertex>> res = std::make_shared<std::vector<Vertex>>(std::initializer_list<Vertex>{ startLinePos, endLinePos });
    std::shared_ptr<GeometryData> geom_data = std::make_shared<GeometryData>(res);
    return geom_data;
}

std::shared_ptr<GeometryData> Point::ConvertPosToPoint(glm::vec3 _Pos) {
    std::shared_ptr<std::vector<Vertex>> res = std::make_shared<std::vector<Vertex>>(std::initializer_list<Vertex>{ {.position = _Pos} });
    std::shared_ptr<GeometryData> geom_data = std::make_shared<GeometryData>(res);
    return geom_data;
}

std::shared_ptr<GeometryData> Cube::MakeCube(float _width = 1, float _height = 1, float _depth = 1) {
    float halfWidth = _width / 2.0f;
    float halfHeight = _height / 2.0f;
    float halfDepth = _depth / 2.0f;

    std::vector<glm::vec3> positions = {
        {-halfWidth, -halfHeight, -halfDepth},
        {halfWidth, -halfHeight, -halfDepth},
        {halfWidth, halfHeight, -halfDepth},
        {-halfWidth, halfHeight, -halfDepth},
        {-halfWidth, -halfHeight, halfDepth},
        {halfWidth, -halfHeight, halfDepth},
        {halfWidth, halfHeight, halfDepth},
        {-halfWidth, halfHeight, halfDepth}
    };

    std::vector<glm::vec3> normals = {
        {0.0f, 0.0f, -1.0f}, // Front face
        {1.0f, 0.0f, 0.0f},  // Right face
        {0.0f, 0.0f, 1.0f},  // Back face
        {-1.0f, 0.0f, 0.0f}, // Left face
        {0.0f, -1.0f, 0.0f},  // Bottom face
        {0.0f, 1.0f, 0.0f},  // Top face
    };

    std::vector<glm::vec2> texCoords = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},

        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},

        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},
        {1.0f, 0.0f},

        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},
        {1.0f, 0.0f},

        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 1.0f},

        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 1.0f},
    };

    std::shared_ptr<std::vector<Vertex>> _vertices = std::make_shared<std::vector<Vertex>>();
    for (size_t i = 0; i < SC::CubeIndices->size(); ++i) {
        Vertex vertex{};
        vertex.position = positions[SC::CubeIndices->data()[i]];
        vertex.normal = normals[i / 6];

        vertex.texCoord = texCoords[i];
        _vertices->push_back(vertex);
    }

    std::shared_ptr<GeometryData> geom_data = std::make_shared<GeometryData>(_vertices);
    return geom_data;
}

std::shared_ptr<GeometryData> Cone::MakeCone(float _height = 1.f, float _radius = 1.f) {
    std::shared_ptr<std::vector<Vertex>> _vertices = std::make_shared<std::vector<Vertex>>();
    _vertices->push_back({ .position = glm::vec3(0.f, _height, 0.f), .texCoord = glm::vec2(0.5f, 1.0f), .normal = glm::vec3(0.f, 1.f, 0.f) }); // Top center vertex

    float zAngle = atan2(_radius, _height);
    float x0 = cos(zAngle);
    float y0 = 0;
    float z0 = sin(zAngle);

    float segmentStep = 2 * PI / CONE_SEGMENTS;

    // rotate (x0,y0,z0) per sector angle
    std::vector<glm::vec3> normals = {};
    for (int i = 0; i <= CONE_SEGMENTS; ++i)
    {
        float sectorAngle = static_cast<float>(i) * segmentStep;
        normals.push_back(glm::vec3(cos(sectorAngle + segmentStep / 2), 0, sin(sectorAngle + segmentStep / 2)));
    }

    for (int i = 0; i <= CONE_SEGMENTS; i++) {
        GLfloat angle = 2.0f * PI * static_cast<GLfloat>(i) / static_cast<GLfloat>(CONE_SEGMENTS);
        GLfloat x = _radius * cos(angle);
        GLfloat z = _radius * sin(angle);

        Vertex vert{};
        vert.position = glm::vec3(x, 0.f, z);
        vert.texCoord = glm::vec2(static_cast<float>(i) / static_cast<float>(CONE_SEGMENTS), 0.f);
        vert.normal = normals[i];
        _vertices->push_back(vert);
    }

    // Add bottom vertex
    _vertices->push_back({ .position = glm::vec3(0.f, 0.f, 0.f), .texCoord = glm::vec2(0.5f, 0.f), .normal = glm::vec3(0.f, -1.f, 0.f) });

    std::shared_ptr<std::vector<uint32_t>> _indices = std::make_shared<std::vector<uint32_t>>();
    for (int i = 1; i <= CONE_SEGMENTS; ++i) {
        _indices->push_back(0); // Top center vertex
        _indices->push_back(i);
        _indices->push_back(i + 1);
    }

    // Connect the last vertex to the first vertex to close the top part of the cone
    _indices->push_back(0); // Top center vertex
    _indices->push_back(CONE_SEGMENTS);
    _indices->push_back(1);

    // Bottom indices (triangle fan using center vertex and base vertices)
    for (int i = 1; i <= CONE_SEGMENTS; ++i) {
        _indices->push_back(_vertices->size() - 1); // Bottom center vertex
        _indices->push_back(i);
        _indices->push_back(i + 1);
    }

    std::shared_ptr<GeometryData> geom_data = std::make_shared<GeometryData>(_vertices, _indices);
    return geom_data;
}
