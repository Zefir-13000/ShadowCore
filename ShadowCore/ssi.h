#pragma once
// Standart Shadow Include
#include <Windows.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Core/Classes.h"

constexpr auto PI = 3.141592654;

struct Vertex {
	glm::vec3 position{};
	glm::vec2 texCoord{};
	glm::vec3 normal{};
};

class GeometryData {
public:
	std::shared_ptr<std::vector<Vertex>> vertices = nullptr;
	std::shared_ptr<std::vector<uint32_t>> indices = nullptr;

	bool Inited = false;
	uint32_t VAO = 0, VBO = 0, EBO = 0;
	size_t vertices_count = 0, indices_count = 0;
	RenderType render_type = NORENDER;
	GLenum render_mode = GL_TRIANGLES;

	GeometryData() {
		GeometryData::render_type = NORENDER;
	}

	GeometryData(std::vector<Vertex> _vertices) {
		GeometryData::vertices = std::make_shared<std::vector<Vertex>>(_vertices);
		GeometryData::vertices_count = GeometryData::vertices->size();
		GeometryData::render_type = ARRAY;
		Initialize();
	}
	GeometryData(std::vector<Vertex> _vertices, std::vector<uint32_t> _indices) {
		GeometryData::vertices = std::make_shared<std::vector<Vertex>>(_vertices);
		GeometryData::indices = std::make_shared<std::vector<uint32_t>>(_indices);
		GeometryData::vertices_count = GeometryData::vertices->size();
		GeometryData::indices_count = GeometryData::indices->size();
		GeometryData::render_type = ELEMENT;
		Initialize();
	}
	GeometryData(std::shared_ptr<std::vector<Vertex>> _vertices) {
		GeometryData::vertices = _vertices;
		GeometryData::vertices_count = vertices->size();
		GeometryData::render_type = ARRAY;
		Initialize();
	}
	GeometryData(std::shared_ptr<std::vector<Vertex>> _vertices, std::shared_ptr<std::vector<uint32_t>> _indices) {
		GeometryData::vertices = _vertices;
		GeometryData::indices = _indices;
		GeometryData::vertices_count = GeometryData::vertices->size();
		GeometryData::indices_count = GeometryData::indices->size();
		GeometryData::render_type = ELEMENT;
		Initialize();
	}

	GeometryData(std::vector<Vertex> _vertices, GLenum _render_mode) {
		GeometryData::vertices = std::make_shared<std::vector<Vertex>>(_vertices);
		GeometryData::vertices_count = vertices->size();
		GeometryData::render_type = ARRAY;
		GeometryData::render_mode = _render_mode;
		Initialize();
	}
	GeometryData(std::vector<Vertex> _vertices, std::vector<uint32_t> _indices, GLenum _render_mode) {
		GeometryData::vertices = std::make_shared<std::vector<Vertex>>(_vertices);
		GeometryData::indices = std::make_shared<std::vector<uint32_t>>(_indices);
		GeometryData::vertices_count = GeometryData::vertices->size();
		GeometryData::indices_count = GeometryData::indices->size();
		GeometryData::render_type = ELEMENT;
		GeometryData::render_mode = _render_mode;
		Initialize();
	}
	GeometryData(std::shared_ptr<std::vector<Vertex>> _vertices, GLenum _render_mode) {
		GeometryData::vertices = _vertices;
		GeometryData::vertices_count = vertices->size();
		GeometryData::render_type = ARRAY;
		GeometryData::render_mode = _render_mode;
		Initialize();
	}
	GeometryData(std::shared_ptr<std::vector<Vertex>> _vertices, std::shared_ptr<std::vector<uint32_t>> _indices, GLenum _render_mode) {
		GeometryData::vertices = _vertices;
		GeometryData::indices = _indices;
		GeometryData::vertices_count = GeometryData::vertices->size();
		GeometryData::indices_count = GeometryData::indices->size();
		GeometryData::render_type = ELEMENT;
		GeometryData::render_mode = _render_mode;
		Initialize();
	}

	void Initialize() {
		if (GeometryData::render_type != NORENDER && !Inited) {
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			if (render_type == ELEMENT)
				glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, vertices->data(), GL_STATIC_DRAW);

			if (render_type == ELEMENT) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices_count, indices->data(), GL_STATIC_DRAW);
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
	}

	void UnInitialize() {
		if (Inited) {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
			Inited = false;
		}
	}

	~GeometryData() {
		UnInitialize();
	}
};

struct RenderSequence {
	std::vector<std::shared_ptr<GeometryData>> geoms_data;

	RenderSequence(std::initializer_list<std::shared_ptr<GeometryData>> args) : geoms_data(args) {}
	RenderSequence(std::vector<std::shared_ptr<GeometryData>> args) : geoms_data(args) {}
};