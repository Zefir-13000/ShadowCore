#pragma once
#include "Core/Engine.h"
#include "Object/Mesh.h"

namespace SC {

    extern std::shared_ptr<std::vector<uint32_t>> CubeIndices;
    class Cube : public Mesh {
    private:
        std::shared_ptr<GeometryData> MakeCube(float _width, float _height, float _depth);
    public:
        Cube(float _width = 1.f, float _height = 1.f, float _depth = 1.f) : Mesh("Cube", MakeCube(_width, _height, _depth), MANDATORY) {
            Cube::type = MESH;
            Cube::meshType = CUBE_TYPE;
        }
    };

    constexpr int CONE_SEGMENTS = 18;
    class Cone : public Mesh {
    private:
        std::shared_ptr<GeometryData> MakeCone(float _height, float _radius);
    public:
        Cone(float _height = 1.f, float _radius = 1.f) : Mesh("Cone", MakeCone(_height, _radius), MANDATORY) {
            Cone::type = MESH;
            Cone::meshType = CONE_TYPE;
        }
    };



    class Line : public Mesh {
    private:
        std::shared_ptr<GeometryData> ConvertPosToLine(glm::vec3 _startPos, glm::vec3 _endPos);
    public:
        Line(glm::vec3 _startPos, glm::vec3 _endPos) : Mesh("Line", ConvertPosToLine(_startPos, _endPos), USEFUL) {
            Line::type = MESH;
            Line::meshType = LINE_TYPE;
        }

        void Render() override {
            enginePtr->debug_shader->Activate();
            enginePtr->debug_shader->setVec3("color", glm::vec3(0.8f));
            enginePtr->debug_shader->setMat4("model", glm::mat4(1.0f));
            enginePtr->debug_shader->setMat4("view", enginePtr->level->main_cam->view);
            enginePtr->debug_shader->setMat4("projection", enginePtr->level->main_cam->proj);

            enginePtr->debug_shader->setMat4("model", glm::mat4(1.0));

            glBindVertexArray(VAO);
            glDrawArrays(GL_LINES, 0, 2);
            glBindVertexArray(0);
        }
    };

    class Point : public Mesh {
    private:
        float point_size = 1.f;

        std::shared_ptr<GeometryData> ConvertPosToPoint(glm::vec3 _Pos);
    public:
        Point(glm::vec3 _pos, float _size) : Mesh("Point", ConvertPosToPoint(_pos), USEFUL) {
            Point::type = MESH;
            Point::meshType = POINT_TYPE;
            point_size = _size;
        }

        void Render() override {
            enginePtr->debug_shader->Activate();
            enginePtr->debug_shader->setVec3("color", glm::vec3(0.8f));
            enginePtr->debug_shader->setMat4("model", glm::mat4(1.0f));
            enginePtr->debug_shader->setMat4("view", enginePtr->level->main_cam->view);
            enginePtr->debug_shader->setMat4("projection", enginePtr->level->main_cam->proj);

            enginePtr->debug_shader->setMat4("model", glm::mat4(1.0));

            glBindVertexArray(VAO);
            glPointSize(point_size);
            glDrawArrays(GL_POINTS, 0, 1);
            glPointSize(1);
            glBindVertexArray(0);
        }
    };

};