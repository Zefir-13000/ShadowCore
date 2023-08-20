#pragma once
#include "Core/Engine.h"
#include "Object/Mesh.h"

namespace SC {

    extern std::shared_ptr<std::vector<uint32_t>> CubeIndices;
    class Cube : public Mesh {
    public:
        static std::shared_ptr<GeometryData> MakeCube(float _width, float _height, float _depth);
        Cube(float _width = 1.f, float _height = 1.f, float _depth = 1.f) : Mesh("Cube", MakeCube(_width, _height, _depth), MANDATORY) {
            Cube::type = MESH;
            Cube::meshType = CUBE_TYPE;
        }
    };

    constexpr int CONE_SEGMENTS = 18;
    class Cone : public Mesh {
    public:
        static std::shared_ptr<GeometryData> MakeCone(float _height, float _radius);
        Cone(float _height = 1.f, float _radius = 1.f) : Mesh("Cone", MakeCone(_height, _radius), MANDATORY) {
            Cone::type = MESH;
            Cone::meshType = CONE_TYPE;
        }
    };

    class Plane : public Mesh {
    public:
        static std::shared_ptr<GeometryData> MakePlane(float _width, float _height);
        Plane(float _width = 1.f, float _height = 1.f) : Mesh("Plane", MakePlane(_width, _height), MANDATORY) {
            Plane::type = MESH;
            Plane::meshType = PLANE_TYPE;
        }
    };

    class Line : public Mesh {
    public:
        static std::shared_ptr<GeometryData> MakeLine(glm::vec3 _startPos, glm::vec3 _endPos);
        Line(glm::vec3 _startPos, glm::vec3 _endPos) : Mesh("Line", MakeLine(_startPos, _endPos), USEFUL) {
            Line::type = MESH;
            Line::meshType = LINE_TYPE;
        }
    };

    class Arrow : public Mesh {
    public:
        static std::shared_ptr<RenderSequence> MakeArrow(float _length);
        Arrow(float _length);
    };

    class Point : public Mesh {
    private:
        float point_size = 1.f;
    public:
        static std::shared_ptr<GeometryData> MakePoint(glm::vec3 _Pos);
        Point(glm::vec3 _pos, float _size) : Mesh("Point", MakePoint(_pos), USEFUL) {
            Point::type = MESH;
            Point::meshType = POINT_TYPE;
            point_size = _size;
        }

        void Render() override {
            enginePtr->debug_shader->Activate();
            enginePtr->debug_shader->setValue("color", glm::vec3(0.8f));
            enginePtr->debug_shader->setValue("MVP", enginePtr->level->main_cam->proj * enginePtr->level->main_cam->view);

            for (std::shared_ptr<GeometryData> geom_data : renderSeq->geoms_data) {
                glBindVertexArray(geom_data->VAO);
                glPointSize(point_size);
                if (geom_data->render_type == ELEMENT) {
                    glDrawElements(geom_data->render_mode, static_cast<GLsizei>(geom_data->indices_count), GL_UNSIGNED_INT, 0);
                }
                else if (geom_data->render_type == ARRAY) {
                    glDrawArrays(geom_data->render_mode, 0, static_cast<GLsizei>(geom_data->vertices_count));
                }
                glPointSize(1);
                glBindVertexArray(0);
            }
        }
    };

};