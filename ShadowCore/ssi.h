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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

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

struct GeometryData {
	std::shared_ptr<std::vector<Vertex>> vertices = nullptr;
	std::shared_ptr<std::vector<uint32_t>> indices = nullptr;

	size_t vertices_count = 0, indices_count = 0;
	RenderType render_type = ARRAY;

	GeometryData(std::vector<Vertex> _vertices) {
		GeometryData::vertices = std::make_shared<std::vector<Vertex>>(_vertices);
		GeometryData::vertices_count = vertices->size();
		GeometryData::render_type = ARRAY;
	}
	GeometryData(std::vector<Vertex> _vertices, std::vector<uint32_t> _indices) {
		GeometryData::vertices = std::make_shared<std::vector<Vertex>>(_vertices);
		GeometryData::indices = std::make_shared<std::vector<uint32_t>>(_indices);
		GeometryData::vertices_count = GeometryData::vertices->size();
		GeometryData::indices_count = GeometryData::indices->size();
		GeometryData::render_type = ELEMENT;
	}
	GeometryData(std::shared_ptr<std::vector<Vertex>> _vertices) {
		GeometryData::vertices = _vertices;
		GeometryData::vertices_count = vertices->size();
		GeometryData::render_type = ARRAY;
	}
	GeometryData(std::shared_ptr<std::vector<Vertex>> _vertices, std::shared_ptr<std::vector<uint32_t>> _indices) {
		GeometryData::vertices = _vertices;
		GeometryData::indices = _indices;
		GeometryData::vertices_count = GeometryData::vertices->size();
		GeometryData::indices_count = GeometryData::indices->size();
		GeometryData::render_type = ELEMENT;
	}
};