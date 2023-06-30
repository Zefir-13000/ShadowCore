#pragma once
#include "ssi.h"
#include "Object.h"
#include "Shader.h"
#include "Transform.h"
#include "Texture.h"
#include "Material.h"
#include "RenderObject.h"
#include "RayObjects.h"

class Mesh : public RenderObject {
private:
    // none
protected:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
public:
    MeshType meshType = MODEL;

	Mesh(std::string _name, std::vector<float>& _vertices);
    Mesh(std::string _name, std::vector<Vertex>& _vertices);

    Mesh(std::string _name, std::vector<float> _vertices, UseLessType _u_type);
    Mesh(std::string _name, std::vector<Vertex> _vertices, UseLessType _u_type);

	Transform local_transform, transform;
};

class Model : public Mesh {
private:
    // none
public:
    void SetMaterial(std::shared_ptr<Material> _material);

    std::shared_ptr<Material> material;

    void Render() override;

    Model(std::string _name, std::vector<float>& _vertices);
    Model(std::string _name, std::vector<Vertex>& _vertices);

    std::shared_ptr<AABB> aabb_box = nullptr;
};

class Line : public Mesh {
private:
    std::vector<Vertex> ConvertPosToLine(glm::vec3 _startPos, glm::vec3 _endPos);
public:
    Line(glm::vec3 _startPos, glm::vec3 _endPos) : Mesh("Line", ConvertPosToLine(_startPos, _endPos), USEFUL) {
        Line::type = SLINE;
    }

    void Render() override {
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
    }
};

class Point : public Mesh {
private:
    float point_size = 1.f;

    std::vector<Vertex> ConvertPosToPoint(glm::vec3 _Pos);
public:
    Point(glm::vec3 _pos, float _size) : Mesh("Point", ConvertPosToPoint(_pos), USEFUL) {
        Point::type = SPOINT;
        point_size = _size;
    }

    void Render() override {
        glBindVertexArray(VAO);
        glPointSize(point_size);
        glDrawArrays(GL_POINTS, 0, 1);
        glPointSize(1);
        glBindVertexArray(0);
    }
};
