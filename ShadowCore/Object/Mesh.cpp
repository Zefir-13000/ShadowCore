#include "Mesh.h"

SC::Mesh::Mesh(std::string _name, std::shared_ptr<std::vector<float>> _vertices) : RenderObject(_name, _vertices) {
    type = MESH;
}

SC::Mesh::Mesh(std::string _name, std::shared_ptr<std::vector<Vertex>> _vertices) : RenderObject(_name, _vertices) {
    type = MESH;
}

SC::Mesh::Mesh(std::string _name, std::shared_ptr<std::vector<float>> _vertices, UseLessType _u_type) : RenderObject(_name, _vertices) {
    type = MESH;
}

SC::Mesh::Mesh(std::string _name, std::shared_ptr<std::vector<Vertex>> _vertices, UseLessType _u_type) : RenderObject(_name, _vertices) {
    type = MESH;
}

void SC::Mesh::SetMaterial(std::shared_ptr<Material> material) {
    Mesh::material = material;
}

void SC::Mesh::Render() {
    RenderComponents();
    if (Mesh::Inited && Mesh::render_shader != nullptr) {
        Mesh::render_shader->Activate();

        if (Mesh::material != nullptr && Mesh::material->material_type == SMATERIAL) {
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
        //RenderObject::render_shader->setMat4("projection", enginePtr->level->main_cam->proj);
        //RenderObject::render_shader->setMat4("view", enginePtr->level->main_cam->view);

        glBindVertexArray(VAO);
        if (Mesh::render_type == ELEMENT) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Mesh::indices_count), GL_UNSIGNED_INT, 0);
        }
        else if (Mesh::render_type == ARRAY) {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(Mesh::vertices_count));
        }
        glBindVertexArray(0);
    }
    else if (Mesh::render_shader == nullptr) {
        std::cerr << "ERROR::RENDER_OBJECT::RENDER_SHADER - RENDER_OBJECT (" << name << ") at 0x" << std::hex << this << " - RENDER_SHADER is NULL" << std::endl;
    }
}