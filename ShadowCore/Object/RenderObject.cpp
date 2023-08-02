#include "RenderObject.h"
#include "Core/Engine.h"

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

void RenderObject::Initialize(std::shared_ptr<GeometryData> _geom_data) {
    RenderObject::type = RENDER_OBJECT;
    RenderObject::geometry_data = _geom_data;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    if (_geom_data->render_type == ELEMENT)
        glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _geom_data->vertices_count, _geom_data->vertices->data(), GL_STATIC_DRAW);

    if (_geom_data->render_type == ELEMENT) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_geom_data->indices) * _geom_data->indices_count, _geom_data->indices->data(), GL_STATIC_DRAW);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    Inited = true;
}

void RenderObject::UnInitialize() {
    if (Inited) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        Inited = false;
    }
}


RenderObject::RenderObject(std::string _name, std::shared_ptr<GeometryData> _geom_data, UseLessType _u_type = USELESS) {
    RenderObject::name = _name;
    RenderObject::u_type = _u_type;
    Initialize(_geom_data);
}

RenderObject::RenderObject(std::shared_ptr<GeometryData> _geom_data, UseLessType _u_type = USELESS) {
    RenderObject::u_type = _u_type;
    Initialize(_geom_data);
}


void RenderObject::SetShader(std::shared_ptr<Shader> shader) {
    RenderObject::render_shader = shader;
}


void RenderObject::Render() {
    if (RenderObject::Inited && RenderObject::render_shader != nullptr) {
        RenderObject::render_shader->Activate();

        RenderObject::render_shader->setMat4("model", glm::mat4(1.0));

        glBindVertexArray(VAO);
        if (RenderObject::geometry_data->render_type == ELEMENT) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(RenderObject::geometry_data->indices_count), GL_UNSIGNED_INT, 0);
        }
        else if (RenderObject::geometry_data->render_type == ARRAY) {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(RenderObject::geometry_data->vertices_count));
        }
        glBindVertexArray(0);
    }
    else if (RenderObject::render_shader == nullptr) {
        std::cerr << "ERROR::RENDER_OBJECT::RENDER_SHADER - RENDER_OBJECT (" << name << ") at 0x" << std::hex << this << " - RENDER_SHADER is NULL" << std::endl;
        RenderObject::render_shader = enginePtr->standart_render_shader;
    }
}

void RenderObject::RenderComponents() {
    if (RenderObject::Inited) {
        for (std::shared_ptr<Component> component : components) {
            if (component->component_type == RENDER_COMPONENT) {
                enginePtr->debug_shader->Activate();
                enginePtr->debug_shader->setVec3("color", glm::vec3(0.8f));
                enginePtr->debug_shader->setMat4("model", glm::mat4(1.0f));
                enginePtr->debug_shader->setMat4("view", enginePtr->level->main_cam->view);
                enginePtr->debug_shader->setMat4("projection", enginePtr->level->main_cam->proj);

                component->Render();
            }
        }
    }
    else if (RenderObject::render_shader == nullptr) {
        std::cerr << "ERROR::RENDER_OBJECT::RENDER_SHADER - RENDER_OBJECT (" << name << ") at 0x" << std::hex << this << " - RENDER_SHADER is NULL" << std::endl;
        RenderObject::render_shader = enginePtr->standart_render_shader;
    }
}