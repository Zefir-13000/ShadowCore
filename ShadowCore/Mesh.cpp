#include "Mesh.h"

Mesh::Mesh(std::vector<float>& vertices, RenderType render_type) {
    Mesh::render_type = render_type;
    Mesh::type = MESH;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    vertices_count = static_cast<unsigned int>(vertices.size());

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
        std::cerr << "INVALID RENDER TYPE (" << RenderTypeStr[render_type] << "). UNABLE TO LOAD MESH \"" << Mesh::name << "\"" << std::endl;
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

    vertices_count = static_cast<unsigned int>(vertices.size());
    indices_count = static_cast<unsigned int>(indices.size());

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
        std::cerr << "INVALID RENDER TYPE (" << RenderTypeStr[render_type] << "). UNABLE TO LOAD MESH \"" << Mesh::name << "\"" << std::endl;
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

    vertices_count = static_cast<unsigned int>(vertices.size());

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
        std::cerr << "INVALID RENDER TYPE (" << RenderTypeStr[render_type] << "). UNABLE TO LOAD MESH \"" << Mesh::name << "\"" << std::endl;
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

    vertices_count = static_cast<unsigned int>(vertices.size());
    indices_count = static_cast<unsigned int>(indices.size());

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
        std::cerr << "INVALID RENDER TYPE (" << RenderTypeStr[render_type] << "). UNABLE TO LOAD MESH \"" << Mesh::name << "\"" << std::endl;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        Inited = false;
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    Inited = true;
}

void Mesh::SetTextures(std::vector<std::shared_ptr<Texture>> textures) {
    Mesh::textures = textures;
}

void Mesh::Render(std::unique_ptr<Shader>& shader) {
    if (Mesh::Inited) {
        shader->Activate();
        for (std::shared_ptr<Texture>& texture : textures) {
            if (texture->type == DIFFUSE)
                texture->Bind(shader, 0);
            else if (texture->type == SPECULAR)
                texture->Bind(shader, 1);
        }

        glBindVertexArray(VAO);
        if (render_type == ELEMENT || render_type == ELEMENT_CUSTOM) {
            glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
        }
        else if (render_type == ARRAY || render_type == ARRAY_CUSTOM)
            glDrawArrays(GL_TRIANGLES, 0, vertices_count); // Need replace to Vertex struct
        glBindVertexArray(0);
    }
}