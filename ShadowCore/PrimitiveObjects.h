#pragma once
#include "Mesh.h"
#include "Engine.h"

namespace SC {
    class Cone : public Mesh {
    private:
        std::vector<Vertex> MakeCone(float _height, float _radius);
    public:
        Cone(float _height, float _radius) : Mesh("Cone", MakeCone(_height, _radius), MANDATORY) {
            Cone::type = MESH;
            Cone::meshType = CONE_TYPE;
        }

        void Render() override {
            if (RenderObject::Inited && RenderObject::render_shader != nullptr) {
                if (Mesh::material != nullptr && Mesh::material->type == MATERIAL) {
                    std::shared_ptr<SMaterial> matColor = std::dynamic_pointer_cast<SMaterial>(Mesh::material);

                    Mesh::render_shader->setVec3("material.Ambient", matColor->Ambient);
                    Mesh::render_shader->setVec3("material.Diffuse", matColor->Diffuse);
                    Mesh::render_shader->setVec3("material.Specular", matColor->Specular);
                    Mesh::render_shader->setVec3("material.Emission", matColor->Emission);
                    Mesh::render_shader->setFloat("material.Shininess", matColor->Shininess);

                    if (matColor->diffuse_texture != nullptr) {
                        Mesh::render_shader->setInt("has_diffuse_texture", 1);
                        Mesh::render_shader->setInt("diffuse_texture", 0);
                        matColor->diffuse_texture->Bind(Mesh::render_shader, 0);
                    }
                    else {
                        Mesh::render_shader->setInt("has_diffuse_texture", 0);
                    }
                    if (matColor->specular_texture != nullptr) {
                        Mesh::render_shader->setInt("has_specular_texture", 1);
                        Mesh::render_shader->setInt("specular_texture", 1);
                        matColor->specular_texture->Bind(Mesh::render_shader, 1);
                    }
                    else {
                        Mesh::render_shader->setInt("has_specular_texture", 0);
                    }
                    if (matColor->emission_texture != nullptr) {
                        Mesh::render_shader->setInt("has_emission_texture", 1);
                        Mesh::render_shader->setInt("emission_texture", 2);
                        matColor->emission_texture->Bind(Mesh::render_shader, 2);
                    }
                    else {
                        Mesh::render_shader->setInt("has_emission_texture", 0);
                    }
                }
                else {
                    Mesh::render_shader->setVec3("material.Ambient", glm::vec3(0.1f));
                    Mesh::render_shader->setVec3("material.Diffuse", glm::vec3(1.f));
                    Mesh::render_shader->setVec3("material.Specular", glm::vec3(0.5f));
                    Mesh::render_shader->setVec3("material.Emission", glm::vec3(0.0f));
                    Mesh::render_shader->setFloat("material.Shininess", 32.f);

                    Mesh::render_shader->setInt("has_diffuse_texture", 0);
                    Mesh::render_shader->setInt("has_specular_texture", 0);
                    Mesh::render_shader->setInt("has_emission_texture", 0);
                }

                Mesh::render_shader->setMat4("model", Mesh::transform.model);

                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(vertices_count));
                glBindVertexArray(0);
            }
        }
    };



    class Line : public Mesh {
    private:
        std::vector<Vertex> ConvertPosToLine(glm::vec3 _startPos, glm::vec3 _endPos);
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

        std::vector<Vertex> ConvertPosToPoint(glm::vec3 _Pos);
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