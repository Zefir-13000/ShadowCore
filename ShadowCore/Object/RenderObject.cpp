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

    RenderObject::render_shader = enginePtr->debug_shader;
    RenderObject::transform->this_obj = this;
}

RenderObject::RenderObject(std::shared_ptr<GeometryData> _geom_data, UseLessType _u_type = USELESS) : Object() {
    RenderObject::u_type = _u_type;
    RenderObject::type = RENDER_OBJECT;
    RenderObject::renderSeq = std::make_shared<RenderSequence>(std::initializer_list<std::shared_ptr<GeometryData>>{ _geom_data });

    RenderObject::render_shader = enginePtr->debug_shader;
    RenderObject::transform->this_obj = this;
}

RenderObject::RenderObject(std::string _name, std::shared_ptr<RenderSequence> _render_seq, UseLessType _u_type) : Object(_name) {
    RenderObject::name = _name;
    RenderObject::u_type = _u_type;
    RenderObject::type = RENDER_OBJECT;
    RenderObject::renderSeq = _render_seq;

    RenderObject::render_shader = enginePtr->debug_shader;
    RenderObject::transform->this_obj = this;
}

RenderObject::RenderObject(std::shared_ptr<RenderSequence> _render_seq, UseLessType _u_type) : Object() {
    RenderObject::u_type = _u_type;
    RenderObject::type = RENDER_OBJECT;
    RenderObject::renderSeq = _render_seq;

    RenderObject::render_shader = enginePtr->debug_shader;
    RenderObject::transform->this_obj = this;
}


void RenderObject::SetShader(std::shared_ptr<Shader> shader) {
    RenderObject::render_shader = shader;
}

void RenderObject::Render(std::shared_ptr<Shader> _render_shader, bool ignore_inputs) {
    if (Core::isEnableEditor) //  && this->getId() == Core::selected_ObjectID
        RenderComponents();
    if (_render_shader != nullptr) {
        _render_shader->Activate();

        _render_shader->setValue("MVP", Core::Engine->level->main_cam->proj * Core::Engine->level->main_cam->view * RenderObject::transform->GetMatrix());
        _render_shader->setValue("model", RenderObject::transform->GetMatrix());
        _render_shader->setValue("color", glm::vec3(0.8f));

        if (Core::Engine->level->shadows.size() > 0) {
            _render_shader->setValue("lightSpaceMatrix", Core::Engine->level->shadows[0]->GetRenderCam()->GetPVMatrix());
            _render_shader->setValue("shadow_map", 5);
            Core::Engine->level->shadows[0]->Bind(5, false);
            _render_shader->setValue("has_shadow_map", 1);
        }

        if (Core::isEnableEditor) {
            _render_shader->setValue("ObjectIndex", static_cast<uint32_t>(getId()));
        }

        if (!ignore_inputs && RenderObject::shader_input != nullptr) {
            RenderObject::shader_input->BindAllToShader();
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
    if (Core::isEnableEditor) //  && this->getId() == Editor::selected_ObjectID
        RenderComponents();
    if (RenderObject::render_shader != nullptr) {
        RenderObject::render_shader->Activate();

        RenderObject::render_shader->setValue("MVP", Core::Engine->level->main_cam->proj * Core::Engine->level->main_cam->view * RenderObject::transform->GetMatrix());
        RenderObject::render_shader->setValue("model", RenderObject::transform->GetMatrix());
        RenderObject::render_shader->setValue("color", glm::vec3(0.8f));

        if (Core::isEnableEditor) {
            RenderObject::render_shader->setValue("ObjectIndex", static_cast<uint32_t>(getId()));
        }

        if (Core::Engine->level->shadows.size() > 0) {
            RenderObject::render_shader->setValue("lightSpaceMatrix", Core::Engine->level->shadows[0]->GetRenderCam()->GetPVMatrix());
            RenderObject::render_shader->setValue("shadow_map", 5);
            Core::Engine->level->shadows[0]->Bind(5, false);
            RenderObject::render_shader->setValue("has_shadow_map", 1);
        }

        if (RenderObject::shader_input != nullptr) {
            RenderObject::shader_input->BindAllToShader();
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
    else if (RenderObject::render_shader == nullptr) {
        RenderObject::render_shader = enginePtr->debug_shader;
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
            enginePtr->debug_shader->Activate();
            enginePtr->debug_shader->setValue("color", glm::vec3(0.8f));
            enginePtr->debug_shader->setValue("MVP", Core::Engine->level->main_cam->proj * Core::Engine->level->main_cam->view);

            component->Render();
        }
    }
}

void RenderObject::UpdateComponents() {
    for (std::shared_ptr<Component> component : components) {
        component->Update();
    }
}

template <typename T> 
void RenderObject::AddShaderInput(std::string& name, const T& value) {
    if (RenderObject::shader_input == nullptr) {
        if (RenderObject::render_shader == nullptr) {
            RenderObject::render_shader = enginePtr->debug_shader;
        }
        RenderObject::shader_input = std::make_shared<ShaderInputCollection>(RenderObject::render_shader);
    }
    //RenderObject::shader_input->AddInput(name, value);
}