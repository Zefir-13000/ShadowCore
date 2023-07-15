#include "PrimitiveObjects.h"

std::vector<Vertex> SC::Line::ConvertPosToLine(glm::vec3 _startPos, glm::vec3 _endPos) {
    Vertex startLinePos{}, endLinePos{};
    startLinePos.position = _startPos;
    endLinePos.position = _endPos;
    std::vector<Vertex> res = { startLinePos, endLinePos };
    return res;
}

std::vector<Vertex> SC::Point::ConvertPosToPoint(glm::vec3 _Pos) {
    std::vector<Vertex> res = { {.position = _Pos} };
    return res;
}

std::vector<Vertex> SC::Cone::MakeCone(float _height, float _radius) {
    std::vector<Vertex> res = { {.position = glm::vec3(0.f, _height, 0.f)} };
    for (float i = -PI; i < PI; i += PI / 15.f) {
        Vertex vert = {};
        vert.position = glm::vec3(sin(i), 0.f, cos(i)) * _radius;
        res.push_back(vert);
    }

    return res;
}
