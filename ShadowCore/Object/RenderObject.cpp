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

void RenderObject::Initialize(std::shared_ptr<std::vector<Vertex>> _vertices, std::shared_ptr<std::vector<uint32_t>> _indices) {
    RenderObject::type = RENDER_OBJECT;
    RenderObject::render_type = ELEMENT;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    vertices_count = _vertices->size();
    indices_count = _indices->size();

    vertices = _vertices;
    indices = _indices;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, _vertices->data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices) * indices_count, _indices->data(), GL_STATIC_DRAW);

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

void RenderObject::Initialize(std::shared_ptr<std::vector<Vertex>> _vertices) {
    RenderObject::type = RENDER_OBJECT;
    RenderObject::render_type = ARRAY;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    vertices_count = _vertices->size();
    vertices = _vertices;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, _vertices->data(), GL_STATIC_DRAW);


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


RenderObject::RenderObject(std::shared_ptr<std::vector<float>> _vertices) {
    std::shared_ptr<std::vector<Vertex>> vertices = ArrayToVertex(_vertices);
    Initialize(vertices);
}

RenderObject::RenderObject(std::shared_ptr<std::vector<float>> _vertices, std::shared_ptr<std::vector<uint32_t>> _indices) {
    std::shared_ptr<std::vector<Vertex>> vertices = ArrayToVertex(_vertices);
    Initialize(vertices, _indices);
}

RenderObject::RenderObject(std::string _name, std::shared_ptr<std::vector<float>> _vertices) : RenderObject(_vertices) {
    RenderObject::name = _name;
}

RenderObject::RenderObject(std::string _name, std::shared_ptr<std::vector<float>> _vertices, std::shared_ptr<std::vector<uint32_t>> _indices) : RenderObject(_vertices, _indices) {
    RenderObject::name = _name;
}

RenderObject::RenderObject(std::shared_ptr<std::vector<Vertex>> _vertices) {
    Initialize(_vertices);
}

RenderObject::RenderObject(std::shared_ptr<std::vector<Vertex>> _vertices, std::shared_ptr<std::vector<uint32_t>> _indices) {
    Initialize(_vertices, _indices);
}

RenderObject::RenderObject(std::string _name, std::shared_ptr<std::vector<Vertex>> _vertices) : RenderObject(_vertices) {
    RenderObject::name = _name;
}

RenderObject::RenderObject(std::string _name, std::shared_ptr<std::vector<Vertex>> _vertices, std::shared_ptr<std::vector<uint32_t>> _indices) : RenderObject(_vertices, _indices) {
    RenderObject::name = _name;
}


void RenderObject::SetShader(std::shared_ptr<Shader> shader) {
    RenderObject::render_shader = shader;
}


void RenderObject::Render() {
    if (RenderObject::Inited && RenderObject::render_shader != nullptr) {
        RenderObject::render_shader->Activate();

        RenderObject::render_shader->setMat4("model", glm::mat4(1.0));

        glBindVertexArray(VAO);
        if (render_type == ELEMENT) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_count), GL_UNSIGNED_INT, 0);
        }
        else if (render_type == ARRAY) {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_count));
        }
        glBindVertexArray(0);
    }
    else if (RenderObject::render_shader == nullptr) {
        std::cerr << "ERROR::RENDER_OBJECT::RENDER_SHADER - RENDER_OBJECT (" << name << ") at 0x" << std::hex << this << " - RENDER_SHADER is NULL" << std::endl;
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

                enginePtr->debug_shader->setMat4("model", glm::mat4(1.0));
                component->Render();
            }
        }
    }
    else if (RenderObject::render_shader == nullptr) {
        std::cerr << "ERROR::RENDER_OBJECT::RENDER_SHADER - RENDER_OBJECT (" << name << ") at 0x" << std::hex << this << " - RENDER_SHADER is NULL" << std::endl;
    }
}