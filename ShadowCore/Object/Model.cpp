#include "Model.h"

SC::Model::Model(std::string _name, std::shared_ptr<std::vector<float>> _vertices) : Mesh(_name, _vertices) {
    type = MESH;
    Model::vertices = ArrayToVertex(_vertices);
}

SC::Model::Model(std::string _name, std::shared_ptr<std::vector<Vertex>> _vertices) : Mesh(_name, _vertices) {
    type = MESH;
    Model::vertices = _vertices;
}

SC::Model::Model(std::string _name, std::shared_ptr<std::vector<float>> _vertices, UseLessType _u_type) : Mesh(_name, _vertices) {
    type = MESH;
    Model::vertices = ArrayToVertex(_vertices);
}

SC::Model::Model(std::string _name, std::shared_ptr<std::vector<Vertex>> _vertices, UseLessType _u_type) : Mesh(_name, _vertices) {
    type = MESH;
    Model::vertices = _vertices;
}

void SC::Model::Render() {

    if (Model::Inited && Model::render_shader != nullptr) {
        Model::render_shader->Activate();

        if (Model::material != nullptr && Model::material->type == MATERIAL) {
            std::shared_ptr<SMaterial> matColor = std::dynamic_pointer_cast<SMaterial>(Model::material);

            Model::render_shader->setVec3("material.Ambient", matColor->Ambient);
            Model::render_shader->setVec3("material.Diffuse", matColor->Diffuse);
            Model::render_shader->setVec3("material.Specular", matColor->Specular);
            Model::render_shader->setVec3("material.Emission", matColor->Emission);
            Model::render_shader->setFloat("material.Shininess", matColor->Shininess);

            if (matColor->diffuse_texture != nullptr) {
                Model::render_shader->setInt("has_diffuse_texture", 1);
                Model::render_shader->setInt("diffuse_texture", 0);
                matColor->diffuse_texture->Bind(Model::render_shader, 0);
            }
            else {
                Model::render_shader->setInt("has_diffuse_texture", 0);
            }
            if (matColor->specular_texture != nullptr) {
                Model::render_shader->setInt("has_specular_texture", 1);
                Model::render_shader->setInt("specular_texture", 1);
                matColor->specular_texture->Bind(Model::render_shader, 1);
            }
            else {
                Model::render_shader->setInt("has_specular_texture", 0);
            }
            if (matColor->emission_texture != nullptr) {
                Model::render_shader->setInt("has_emission_texture", 1);
                Model::render_shader->setInt("emission_texture", 2);
                matColor->emission_texture->Bind(Model::render_shader, 2);
            }
            else {
                Model::render_shader->setInt("has_emission_texture", 0);
            }
        }
        else {
            Model::render_shader->setVec3("material.Ambient", glm::vec3(0.1f));
            Model::render_shader->setVec3("material.Diffuse", glm::vec3(1.f));
            Model::render_shader->setVec3("material.Specular", glm::vec3(0.5f));
            Model::render_shader->setVec3("material.Emission", glm::vec3(0.0f));
            Model::render_shader->setFloat("material.Shininess", 32.f);

            Model::render_shader->setInt("has_diffuse_texture", 0);
            Model::render_shader->setInt("has_specular_texture", 0);
            Model::render_shader->setInt("has_emission_texture", 0);
        }

        Model::render_shader->setMat4("model", Model::transform.model);
        //RenderObject::render_shader->setMat4("projection", enginePtr->level->main_cam->proj);
        //RenderObject::render_shader->setMat4("view", enginePtr->level->main_cam->view);

        glBindVertexArray(VAO);
        if (Model::render_type == ELEMENT) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Model::indices_count), GL_UNSIGNED_INT, 0);
        }
        else if (Model::render_type == ARRAY) {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(Model::vertices_count));
        }
        glBindVertexArray(0);
    }
    else if (Model::render_shader == nullptr) {
        std::cerr << "ERROR::RENDER_OBJECT::RENDER_SHADER - RENDER_OBJECT (" << name << ") at 0x" << std::hex << this << " - RENDER_SHADER is NULL" << std::endl;
    }
}