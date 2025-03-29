#include <string>
#include <sstream>
#include <fstream>

#include "exlib/opengl/shader.hpp"
#include "exlib/core/exception.hpp"

namespace ex::gl {

Shader::ProgramSource Shader::ProgramSource::ReadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);

	if (!file.is_open()) {
		throw Exception("Failed to open the shader source file: " + filepath);
	}

	enum class ShaderType : int {
		None = -1,
		Vertex = 0,
		Fragment = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::None;

	while (file >> line) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::Vertex;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::Fragment;
			else
				throw Exception("Invalid shader type: " + line);
		}
		else if (type != ShaderType::None) {
			ss[(int) type] << line << std::endl;
		}
	}

	file.close();

	return { ss[0].str(), ss[1].str() };
}

Shader::ProgramSource Shader::ProgramSource::ReadFromFile(const std::string& vertex, const std::string& fragment) {
	std::ifstream file(vertex);
	std::string line;

	// Vertex shader
	if (!file.is_open()) {
		throw Exception("Failed to open the vertex shader source file: " + vertex);
	}

	std::stringstream vertex_ss;

	while (file >> line) {
		vertex_ss << line << std::endl;
	}

	file.close();

	// Fragment shader
	file.open(fragment);

	std::stringstream fragment_ss;

	if (!file.is_open()) {
		throw Exception("Failed to open the fragment shader source file: " + fragment);
	}

	while (file >> line) {
		fragment_ss << line << std::endl;
	}

	file.close();

	return { vertex_ss.str(), fragment_ss.str() };
}

Shader::Shader(const ProgramSource& source) 
	: id(0) {
	id = create_shader(source);
}

Shader::~Shader() {
	if (id != 0) {
		glDeleteProgram(id);
	}
}

Shader::Shader(Shader&& other) 
	: id(other.id), uniform_locations(std::move(other.uniform_locations)) {
	other.id = 0;
	other.uniform_locations.clear();
}

Shader& Shader::operator=(Shader&& other) {
	if (this != &other) {
		if (id != 0) {
			glDeleteProgram(id);
		}
		id = other.id;
		uniform_locations = std::move(other.uniform_locations);

		other.id = 0;
		other.uniform_locations.clear();
	}
	return *this;
}

GLuint Shader::create_shader(const ProgramSource& source) const {
	GLuint program = glCreateProgram();
	GLuint vs = compile_shader(GL_VERTEX_SHADER, source.vertex);
	GLuint fs = compile_shader(GL_FRAGMENT_SHADER, source.fragment);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}

GLuint Shader::compile_shader(GLenum type, const std::string& source) const {
	GLuint shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	// Check error
	int res = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE) {
		int length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			std::string buffer(length, '\0');
			glGetShaderInfoLog(shader, length, &length, &buffer[0]);
			throw Exception(
				"Failed to compile " +
				std::string(type == GL_VERTEX_SHADER ? "vertex" : "fragment") +
				" shader, message: " + buffer
			);
		}
	}

	return shader;
}

}
