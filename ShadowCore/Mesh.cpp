#include "Mesh.h"

Mesh::Mesh(std::vector<float>& vertices, RenderType render_type) {
    Mesh::render_type = render_type;
    Mesh::type = MESH;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    vertices_count = vertices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices_count, vertices.data(), GL_STATIC_DRAW);

    if (render_type == ARRAY_CUSTOM) {
        // Pos (3) + TexCoords (2)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    else if (render_type == ARRAY) {
        // Pos (3)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
    else {
        std::cerr << "ERROR::MESH::RENDER_TYPE - INVALID RENDER TYPE (" << RenderTypeStr[render_type] << "). UNABLE TO LOAD MESH \"" << Mesh::name << "\"" << std::endl;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        Inited = false;
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    Inited = true;
}

Mesh::Mesh(std::vector<float>& vertices, std::vector<unsigned int>& indices, RenderType render_type) {
    Mesh::render_type = render_type;
    Mesh::type = MESH;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    vertices_count = vertices.size();
    indices_count = indices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices_count, vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * indices_count, indices.data(), GL_STATIC_DRAW);

    if (render_type == ELEMENT_CUSTOM) {
        // Pos (3) + TexCoods (2)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    else if (render_type == ELEMENT) {
        // Pos (3)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
    else {
        std::cerr << "ERROR::MESH::RENDER_TYPE - INVALID RENDER TYPE (" << RenderTypeStr[render_type] << "). UNABLE TO LOAD MESH \"" << Mesh::name << "\"" << std::endl;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        Inited = false;
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    Inited = true;
}

Mesh::Mesh(std::string name, std::vector<float>& vertices, RenderType render_type) {
    Mesh::render_type = render_type;
    Mesh::type = MESH;
    Mesh::name = name;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    vertices_count = vertices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices_count, vertices.data(), GL_STATIC_DRAW);

    if (render_type == ARRAY_CUSTOM) {
        // Pos (3) + TexCoords (2)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    else if (render_type == ARRAY) {
        // Pos (3)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
    else {
        std::cerr << "ERROR::MESH::RENDER_TYPE - INVALID RENDER TYPE (" << RenderTypeStr[render_type] << "). UNABLE TO LOAD MESH \"" << Mesh::name << "\"" << std::endl;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        Inited = false;
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    Inited = true;
}

Mesh::Mesh(std::string name, std::vector<float>& vertices, std::vector<unsigned int>& indices, RenderType render_type) {
    Mesh::render_type = render_type;
    Mesh::type = MESH;
    Mesh::name = name;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    vertices_count = vertices.size();
    indices_count = indices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices_count, vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * indices_count, indices.data(), GL_STATIC_DRAW);

    if (render_type == ELEMENT_CUSTOM) {
        // Pos (3) + TexCoods (2)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    else if (render_type == ELEMENT) {
        // Pos (3)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
    else {
        std::cerr << "ERROR::MESH::RENDER_TYPE - INVALID RENDER TYPE (" << RenderTypeStr[render_type] << "). UNABLE TO LOAD MESH \"" << Mesh::name << "\"" << std::endl;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        Inited = false;
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    Inited = true;
}

void Mesh::SetShader(std::shared_ptr<Shader> shader)
{
    Mesh::render_shader = shader;
}

void Mesh::SetTextures(std::vector<std::shared_ptr<Texture>> textures) {
    Mesh::textures = textures;
}

void Mesh::Render() {
    if (Mesh::Inited && Mesh::render_shader != nullptr) {
        Mesh::render_shader->Activate();
        for (std::shared_ptr<Texture>& texture : textures) {
            if (texture->type == DIFFUSE)
                texture->Bind(Mesh::render_shader, 0);
            else if (texture->type == SPECULAR)
                texture->Bind(Mesh::render_shader, 1);
        }

        Mesh::render_shader->setMat4("model", transform.model);

        glBindVertexArray(VAO);
        if (render_type == ELEMENT || render_type == ELEMENT_CUSTOM) {
            glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
        }
        else if (render_type == ARRAY || render_type == ARRAY_CUSTOM)
            glDrawArrays(GL_TRIANGLES, 0, 36); // Need replace to Vertex struct
        glBindVertexArray(0);
    }
    else if (Mesh::render_shader == nullptr) {
        std::cerr << "ERROR::MESH::RENDER::SHADER - Mesh (" << Mesh::name << ") at 0x" << std::hex << this << " - RENDER_SHADER is NULL" << std::endl;
    }
}