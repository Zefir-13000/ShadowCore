#include "Mesh.h"
#include "Core/Core.h"

using namespace SC;

Mesh::Mesh(std::string _name, std::shared_ptr<GeometryData> _geom_data, UseLessType _u_type = USELESS) : RenderObject(_name, _geom_data, _u_type) {
    Mesh::type = MESH;
    Mesh::u_type = _u_type;
}
Mesh::Mesh(std::string _name, std::shared_ptr<RenderSequence> _render_seq, UseLessType _u_type = USELESS) : RenderObject(_name, _render_seq, _u_type) {
    Mesh::type = MESH;
    Mesh::u_type = _u_type;
}

void Mesh::SetMaterial(std::shared_ptr<Material> _material) {
    Mesh::material = _material;
}

void Mesh::Render() {
    //if (Core::isEnableEditor) //  && this->getId() == Core::selected_ObjectID
    //    RenderComponents();
    if (Mesh::render_data->render_shader != nullptr) {
        Mesh::render_data->render_shader->Activate();

        if (Mesh::material != nullptr && Mesh::material->material_type == SMATERIAL) {
            std::shared_ptr<SMaterial> matColor = std::dynamic_pointer_cast<SMaterial>(Mesh::material);

            Mesh::render_data->render_shader->setValue("material.Ambient", matColor->Ambient);
            Mesh::render_data->render_shader->setValue("material.Diffuse", matColor->Diffuse);
            Mesh::render_data->render_shader->setValue("material.Specular", matColor->Specular);
            Mesh::render_data->render_shader->setValue("material.Emission", matColor->Emission);
            Mesh::render_data->render_shader->setValue("material.Shininess", matColor->Shininess);

            if (matColor->diffuse_texture != nullptr) {
                Mesh::render_data->render_shader->setValue("has_diffuse_texture", 1);
                Mesh::render_data->render_shader->setValue("diffuse_texture", 0);
                matColor->diffuse_texture->Bind(Mesh::render_data->render_shader, 0);
            }
            else {
                Mesh::render_data->render_shader->setValue("has_diffuse_texture", 0);
            }
            if (matColor->specular_texture != nullptr) {
                Mesh::render_data->render_shader->setValue("has_specular_texture", 1);
                Mesh::render_data->render_shader->setValue("specular_texture", 1);
                matColor->specular_texture->Bind(Mesh::render_data->render_shader, 1);
            }
            else {
                Mesh::render_data->render_shader->setValue("has_specular_texture", 0);
            }
            if (matColor->emission_texture != nullptr) {
                Mesh::render_data->render_shader->setValue("has_emission_texture", 1);
                Mesh::render_data->render_shader->setValue("emission_texture", 2);
                matColor->emission_texture->Bind(Mesh::render_data->render_shader, 2);
            }
            else {
                Mesh::render_data->render_shader->setValue("has_emission_texture", 0);
            }
        }
        else {
            Mesh::render_data->render_shader->setValue("material.Ambient", glm::vec3(0.1f));
            Mesh::render_data->render_shader->setValue("material.Diffuse", glm::vec3(1.f));
            Mesh::render_data->render_shader->setValue("material.Specular", glm::vec3(0.5f));
            Mesh::render_data->render_shader->setValue("material.Emission", glm::vec3(0.0f));
            Mesh::render_data->render_shader->setValue("material.Shininess", 32.f);

            Mesh::render_data->render_shader->setValue("color", glm::vec3(0.8f));

            Mesh::render_data->render_shader->setValue("has_diffuse_texture", 0);
            Mesh::render_data->render_shader->setValue("has_specular_texture", 0);
            Mesh::render_data->render_shader->setValue("has_emission_texture", 0);
        }
        
        Mesh::render_data->render_shader->setValue("model", Mesh::transform->GetMatrix());
        Mesh::render_data->render_shader->setValue("MVP", Core::Engine->level->main_cam->proj * Core::Engine->level->main_cam->view * Mesh::transform->GetMatrix());

        if (Core::isEnableEditor) {
            Mesh::render_data->render_shader->setValue("ObjectIndex", static_cast<uint32_t>(getId()));
        }

        // TODO: bind all shadow textures
        std::dynamic_pointer_cast<DirectionalLight>(Core::Engine->level->objects[0])->cascade_shadow_map->Bind();
        if (Mesh::render_data->receive_shadows) {
            Mesh::render_data->render_shader->setValue("receive_shadows", 1);
        }
        else {
            Mesh::render_data->render_shader->setValue("receive_shadows", 0);
        }

        if (Mesh::render_data->shader_input != nullptr) {
            Mesh::render_data->shader_input->BindAllToShader();
        }

        for (std::shared_ptr<GeometryData> geom_data : renderSeq->geoms_data) {
            glBindVertexArray(geom_data->VAO);

            if (geom_data->render_type == ELEMENT) {
                glDrawElements(geom_data->render_mode, static_cast<GLsizei>(geom_data->indices_count), GL_UNSIGNED_INT, 0);
            }
            else if (geom_data->render_type == ARRAY) {
                glDrawArrays(geom_data->render_mode, 0, static_cast<GLsizei>(geom_data->vertices_count));
            }
            glBindVertexArray(0);
        }
    }
    else if (Mesh::render_data->render_shader == nullptr) {
        std::cerr << "WARNING::MESH::RENDER_SHADER - RENDER_OBJECT (" << name << ") at 0x" << std::hex << this << " - RENDER_SHADER is NULL" << std::endl;
        RenderObject::render_data->render_shader = enginePtr->GetDefaultShader();
    }
}