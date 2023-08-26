#include "Shader.h"
#include "Core/Core.h"

using namespace SC;

std::string SHADER_FOLDER("Shaders/");
Shader::Shader(_In_ std::string name) {
	std::string vertCode;
	std::string fragCode;
	std::string geomCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;

	bool gShaderExist = std::filesystem::exists(SHADER_FOLDER + name + ".geom");

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(SHADER_FOLDER + name + ".vert");
		fShaderFile.open(SHADER_FOLDER + name + ".frag");
		if (gShaderExist) {
			gShaderFile.open(SHADER_FOLDER + name + ".geom");
		}
		std::stringstream vShaderStream, fShaderStream, gShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		if (gShaderExist) {
			gShaderStream << gShaderFile.rdbuf();
		}

		vShaderFile.close();
		fShaderFile.close();
		if (gShaderExist) {
			gShaderFile.close();
		}

		vertCode = vShaderStream.str();
		fragCode = fShaderStream.str();
		if (gShaderExist) {
			geomCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_READ" << std::endl;
	}

	const GLchar* vShaderCode = vertCode.c_str();
	const GLchar* fShaderCode = fragCode.c_str();
	const GLchar* gShaderCode = nullptr;
	if (gShaderExist) {
		gShaderCode = geomCode.c_str();
	}

	GLuint vertex, fragment, geometry;
	GLint success;
	GLchar infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	if (gShaderExist) {
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
		};
	}

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex);
	glAttachShader(this->ID, fragment);
	if (gShaderExist) {
		glAttachShader(this->ID, geometry);
	}
	glLinkProgram(this->ID);
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (gShaderExist) {
		glDeleteShader(geometry);
	}
}

void Shader::Activate() {
	glUseProgram(this->ID);
	Core::Engine->engine_active_shader = shared_from_this();
}

Shader::~Shader() {
	glDeleteProgram(this->ID);
}