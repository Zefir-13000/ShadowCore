#include "PrimitiveObjects.h"

using namespace SC;

std::shared_ptr<std::vector<Vertex>> Line::ConvertPosToLine(glm::vec3 _startPos, glm::vec3 _endPos) {
    Vertex startLinePos{ .position = _startPos }, endLinePos{ .position = _endPos };
    std::shared_ptr<std::vector<Vertex>> res = std::make_shared<std::vector<Vertex>>(std::initializer_list<Vertex>{ startLinePos, endLinePos });
    return res;
}

std::shared_ptr<std::vector<Vertex>> Point::ConvertPosToPoint(glm::vec3 _Pos) {
    std::shared_ptr<std::vector<Vertex>> res = std::make_shared<std::vector<Vertex>>(std::initializer_list<Vertex>{ {.position = _Pos} });
    return res;
}

std::shared_ptr<std::vector<Vertex>> Cone::MakeCone(float _height, float _radius) {
    std::shared_ptr<std::vector<Vertex>> res = std::make_shared<std::vector<Vertex>>( std::initializer_list<Vertex>{ {.position = glm::vec3(0.f, _height, 0.f)} });
    for (float i = -PI; i < PI; i += PI / 15.f) {
        Vertex vert = {};
        vert.position = glm::vec3(sin(i), 0.f, cos(i)) * _radius;
        res->push_back(vert);
    }

    return res;
}
