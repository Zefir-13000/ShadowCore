#include "RenderObject.h"
#include "Core/Core.h"

using namespace SC;

std::shared_ptr<std::vector<Vertex>> RenderObject::ArrayToVertex(std::shared_ptr<std::vector<float>> _arr) {
    std::shared_ptr<std::vector<Vertex>> res = std::make_shared<std::vector<Vertex>>();

    for (size_t i = 0; i < _arr->size(); i += 8) {
        float v1 = _arr->data()[i + 0];
        float v2 = _arr->data()[i + 1];
        float v3 = _arr->data()[i + 2];
        float tx = _arr->data()[i + 3];
        float ty = _arr->data()[i + 4];
        float n1 = _arr->data()[i + 5];
        float n2 = _arr->data()[i + 6];
        float n3 = _arr->data()[i + 7];

        Vertex v{};
        v.position = glm::vec3(v1, v2, v3);
        v.texCoord = glm::vec2(tx, ty);
        v.normal = glm::vec3(n1, n2, n3);

        res->push_back(v);
    }
    return res;
}

std::shared_ptr<std::vector<Vertex>> RenderObject::ArrayToVertexPositionOnly(std::shared_ptr<std::vector<float>> _arr) {
    std::shared_ptr<std::vector<Vertex>> res = std::make_shared<std::vector<Vertex>>();

    for (size_t i = 0; i < _arr->size(); i += 3) {
        float v1 = _arr->data()[i + 0];
        float v2 = _arr->data()[i + 1];
        float v3 = _arr->data()[i + 2];

        Vertex v{};
        v.position = glm::vec3(v1, v2, v3);
        v.texCoord = glm::vec2(0.);
        v.normal = glm::vec3(0.);

        res->push_back(v);
    }
    return res;
}

RenderObject::RenderObject(std::string _name, std::shared_ptr<GeometryData> _geom_data, UseLessType _u_type = USELESS) : Object(_name) {
    RenderObject::name = _name;
    RenderObject::u_type = _u_type;
    RenderObject::type = RENDER_OBJECT;
    RenderObject::renderSeq = std::make_shared<RenderSequence>(std::initializer_list<std::shared_ptr<GeometryData>>{ _geom_data });

    RenderObject::render_data = std::make_shared<RenderData>();
    RenderObject::render_data->render_shader = enginePtr->GetDefaultShader();
    RenderObject::transform->this_obj = this;
}

RenderObject::RenderObject(std::shared_ptr<GeometryData> _geom_data, UseLessType _u_type = USELESS) : Object() {
    RenderObject::u_type = _u_type;
    RenderObject::type = RENDER_OBJECT;
    RenderObject::renderSeq = std::make_shared<RenderSequence>(std::initializer_list<std::shared_ptr<GeometryData>>{ _geom_data });

    RenderObject::render_data = std::make_shared<RenderData>();
    RenderObject::render_data->render_shader = enginePtr->GetDefaultShader();
    RenderObject::transform->this_obj = this;
}

RenderObject::RenderObject(std::string _name, std::shared_ptr<RenderSequence> _render_seq, UseLessType _u_type) : Object(_name) {
    RenderObject::name = _name;
    RenderObject::u_type = _u_type;
    RenderObject::type = RENDER_OBJECT;
    RenderObject::renderSeq = _render_seq;

    RenderObject::render_data = std::make_shared<RenderData>();
    RenderObject::render_data->render_shader = enginePtr->GetDefaultShader();
    RenderObject::transform->this_obj = this;
}

RenderObject::RenderObject(std::shared_ptr<RenderSequence> _render_seq, UseLessType _u_type) : Object() {
    RenderObject::u_type = _u_type;
    RenderObject::type = RENDER_OBJECT;
    RenderObject::renderSeq = _render_seq;

    RenderObject::render_data = std::make_shared<RenderData>();
    RenderObject::render_data->render_shader = enginePtr->GetDefaultShader();
    RenderObject::transform->this_obj = this;
}


void RenderObject::SetShader(std::shared_ptr<Shader> shader) {
    RenderObject::render_data->render_shader = shader;
    if (RenderObject::render_data->shader_input != nullptr) {
        RenderObject::render_data->shader_input = std::make_shared<ShaderInputCollection>(shader);
    }
}

void RenderObject::Render(std::shared_ptr<Shader> _render_shader, bool ignore_inputs) {
    //if (Core::isEnableEditor) //  && this->getId() == Core::selected_ObjectID
    //   RenderComponents();
    if (_render_shader != nullptr) {
        _render_shader->Activate();

        _render_shader->setValue("MVP", Core::Engine->level->main_cam->proj * Core::Engine->level->main_cam->view * RenderObject::transform->GetMatrix());
        _render_shader->setValue("model", RenderObject::transform->GetMatrix());
        _render_shader->setValue("color", glm::vec3(0.8f));

        if (RenderObject::render_data->receive_shadows) {
            _render_shader->setValue("receive_shadows", 1);
        }
        else {
            _render_shader->setValue("receive_shadows", 0);
        }

        if (Core::isEnableEditor) {
            _render_shader->setValue("ObjectIndex", static_cast<uint32_t>(getId()));
        }

        if (!ignore_inputs && RenderObject::render_data->shader_input != nullptr) {
            RenderObject::render_data->shader_input->BindAllToShader();
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

    for (std::shared_ptr<Object> child : RenderObject::transform->GetChildren()) {
        if (child && child->IsRenderAble()) {
            std::dynamic_pointer_cast<RenderObject>(child)->Render(_render_shader, ignore_inputs);
        }
    }
}

void RenderObject::Render() {
    //if (Core::isEnableEditor) //  && this->getId() == Editor::selected_ObjectID
    //    RenderComponents();
    if (RenderObject::render_data->render_shader != nullptr) {
        RenderObject::render_data->render_shader->Activate();

        RenderObject::render_data->render_shader->setValue("MVP", Core::Engine->level->main_cam->proj * Core::Engine->level->main_cam->view * RenderObject::transform->GetMatrix());
        RenderObject::render_data->render_shader->setValue("model", RenderObject::transform->GetMatrix());
        RenderObject::render_data->render_shader->setValue("color", glm::vec3(0.8f));

        if (Core::isEnableEditor) {
            RenderObject::render_data->render_shader->setValue("ObjectIndex", static_cast<uint32_t>(getId()));
        }

        if (RenderObject::render_data->receive_shadows) {
            render_data->render_shader->setValue("receive_shadows", 1);
        }
        else {
            render_data->render_shader->setValue("receive_shadows", 0);
        }

        if (RenderObject::render_data->shader_input != nullptr) {
            RenderObject::render_data->shader_input->BindAllToShader();
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
    else if (RenderObject::render_data->render_shader == nullptr) {
        RenderObject::render_data->render_shader = enginePtr->GetDefaultShader();
    }

    for (std::shared_ptr<Object> child : RenderObject::transform->GetChildren()) {
        if (child && child->IsRenderAble()) {
            std::dynamic_pointer_cast<RenderObject>(child)->Render();
        }
    }
}

void RenderObject::RenderComponents() {
    for (std::shared_ptr<Component> component : components) {
        if (component->component_type == RENDER_COMPONENT) {
            enginePtr->GetDebugShader()->Activate();
            enginePtr->engine_active_shader->setValue("color", glm::vec3(0.8f));
            enginePtr->engine_active_shader->setValue("MVP", Core::Engine->level->main_cam->proj * Core::Engine->level->main_cam->view);

            component->Render();
        }
    }
}

void RenderObject::UpdateComponents() {
    for (std::shared_ptr<Component> component : components) {
        component->Update();
    }
}

void RenderObject::Update() {
    RenderObject::UpdateComponents();
    for (std::shared_ptr<Object> child : transform->GetChildren()) {
        child->Update();
    }
}