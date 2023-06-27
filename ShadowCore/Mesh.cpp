#include "Mesh.h"

std::vector<Vertex> Mesh::ArrayToVertex(std::vector<float>& _arr) {
    std::vector<Vertex> res{};

    for (size_t i = 0; i < _arr.size(); i+=8) {
        float v1 = _arr[i + 0];
        float v2 = _arr[i + 1];
        float v3 = _arr[i + 2];
        float tx = _arr[i + 3];
        float ty = _arr[i + 4];
        float n1 = _arr[i + 5];
        float n2 = _arr[i + 6];
        float n3 = _arr[i + 7];

        Vertex v{};
        v.position = glm::vec3(v1, v2, v3);
        v.texCoord = glm::vec2(tx, ty);
        v.normal = glm::vec3(n1, n2, n3);

        res.push_back(v);
    }
    return res;
}

Mesh::Mesh(std::vector<float>& _vertices, RenderType _render_type) {
    Mesh::render_type = _render_type;
    Mesh::type = MESH;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    std::vector<Vertex> vertices = Mesh::ArrayToVertex(_vertices);
    vertices_count = vertices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, vertices.data(), GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    /*std::cerr << "ERROR::MESH::RENDER_TYPE - INVALID RENDER TYPE (" << RenderTypeStr[render_type] << "). UNABLE TO LOAD MESH \"" << Mesh::name << "\"" << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    Inited = false;
    return;*/
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    Inited = true;
}

Mesh::Mesh(std::vector<float>& _vertices, std::vector<unsigned int>& _indices, RenderType _render_type) {
    Mesh::render_type = _render_type;
    Mesh::type = MESH;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    std::vector<Vertex> vertices = Mesh::ArrayToVertex(_vertices);
    vertices_count = vertices.size();
    indices_count = _indices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices) * indices_count, _indices.data(), GL_STATIC_DRAW);

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

Mesh::Mesh(std::string _name, std::vector<float>& _vertices, RenderType _render_type) {
    Mesh::render_type = _render_type;
    Mesh::type = MESH;
    Mesh::name = _name;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    std::vector<Vertex> vertices = Mesh::ArrayToVertex(_vertices);
    vertices_count = vertices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, vertices.data(), GL_STATIC_DRAW);


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

Mesh::Mesh(std::string _name, std::vector<float>& _vertices, std::vector<unsigned int>& _indices, RenderType _render_type) {
    Mesh::render_type = _render_type;
    Mesh::type = MESH;
    Mesh::name = _name;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    std::vector<Vertex> vertices = Mesh::ArrayToVertex(_vertices);
    vertices_count = vertices.size();
    indices_count = _indices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices) * indices_count, _indices.data(), GL_STATIC_DRAW);

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

Mesh::Mesh(std::vector<Vertex>& _vertices, RenderType _render_type) {
    Mesh::render_type = _render_type;
    Mesh::type = MESH;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    vertices_count = _vertices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, _vertices.data(), GL_STATIC_DRAW);


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

Mesh::Mesh(std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices, RenderType _render_type) {
    Mesh::render_type = _render_type;
    Mesh::type = MESH;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    vertices_count = _vertices.size();
    indices_count = _indices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices) * indices_count, _indices.data(), GL_STATIC_DRAW);

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

Mesh::Mesh(std::string _name, std::vector<Vertex>& _vertices, RenderType _render_type) {
    Mesh::render_type = _render_type;
    Mesh::type = MESH;
    Mesh::name = _name;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    vertices_count = _vertices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, _vertices.data(), GL_STATIC_DRAW);


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

Mesh::Mesh(std::string _name, std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices, RenderType _render_type) {
    Mesh::render_type = _render_type;
    Mesh::type = MESH;
    Mesh::name = _name;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    vertices_count = _vertices.size();
    indices_count = _indices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices) * indices_count, _indices.data(), GL_STATIC_DRAW);

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


void Mesh::SetShader(std::shared_ptr<Shader> shader)
{
    Mesh::render_shader = shader;
}

void Mesh::SetMaterial(std::shared_ptr<Material> material) {
    Mesh::material = material;
}

void Mesh::Render() {
    if (Mesh::Inited && Mesh::render_shader != nullptr) {
        Mesh::render_shader->Activate();
        Mesh::render_shader->setVec4("material.Albedo", glm::vec4(1.));
        if (Mesh::material->type == COLOR) {
            std::shared_ptr<MaterialColor> matColor = std::dynamic_pointer_cast<MaterialColor>(Mesh::material);
            Mesh::render_shader->setVec3("material.Albedo", matColor->Albedo);
            Mesh::render_shader->setFloat("material.SpecularStrength", matColor->SpecularStrength);
            Mesh::render_shader->setFloat("material.Shininess", matColor->Shininess);
            Mesh::render_shader->setFloat("material.Ambient", matColor->Ambient);
        }
        else if (Mesh::material->type == TEXTURE) {
            std::shared_ptr<MaterialTexture> matTex = std::dynamic_pointer_cast<MaterialTexture>(Mesh::material);
            std::shared_ptr<Texture> diffuse_tex = matTex->diffuse_texture;
            if (diffuse_tex != nullptr) {
                diffuse_tex->Bind(Mesh::render_shader, 0);
            }
        }
        Mesh::render_shader->setMat4("model", transform.model);

        glBindVertexArray(VAO);
        if (render_type == ELEMENT || render_type == ELEMENT_CUSTOM) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_count), GL_UNSIGNED_INT, 0);
        }
        else if (render_type == ARRAY || render_type == ARRAY_CUSTOM) {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_count));
        }
        glBindVertexArray(0);
    }
    else if (Mesh::render_shader == nullptr) {
        std::cerr << "ERROR::MESH::RENDER_SHADER - Mesh (" << Mesh::name << ") at 0x" << std::hex << this << " - RENDER_SHADER is NULL" << std::endl;
    }
}