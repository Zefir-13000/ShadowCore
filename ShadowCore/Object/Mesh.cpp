#include "Mesh.h"
#include "Core/Core.h"

using namespace SC;

Mesh::Mesh(std::string _name, std::shared_ptr<GeometryData> _geom_data, UseLessType _u_type = USELESS) : RenderObject(_name, _geom_data, _u_type) {
    Mesh::type = MESH;
    Mesh::u_type = _u_type;
}

void Mesh::SetMaterial(std::shared_ptr<Material> _material) {
    Mesh::material = _material;
}

void Mesh::Render() {
    if (Core::isEnableEditor && this->getId() == Core::selected_ObjectID)
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
        Mesh::render_shader->setMat4("MVP", Core::Engine->level->main_cam->proj * Core::Engine->level->main_cam->view * Mesh::transform.model);
        //RenderObject::render_shader->setMat4("projection", enginePtr->level->main_cam->proj);
        //RenderObject::render_shader->setMat4("view", enginePtr->level->main_cam->view);

        glBindVertexArray(VAO);
        if (Mesh::geometry_data->render_type == ELEMENT) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Mesh::geometry_data->indices_count), GL_UNSIGNED_INT, 0);
        }
        else if (Mesh::geometry_data->render_type == ARRAY) {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(Mesh::geometry_data->vertices_count));
        }
        glBindVertexArray(0);
    }
    else if (Mesh::render_shader == nullptr) {
        std::cerr << "WARNING::RENDER_OBJECT::RENDER_SHADER - RENDER_OBJECT (" << name << ") at 0x" << std::hex << this << " - RENDER_SHADER is NULL" << std::endl;
        RenderObject::render_shader = enginePtr->standart_render_shader;
    }
}