#include <glm/gtc/type_ptr.hpp>
#include "exlib/opengl/shader.hpp"
#include "exlib/core/exception.hpp"

namespace ex::gl {

// ============================
// UNIFORM HANDLING
// ============================

GLuint Shader::get_uniform_location(const std::string& name) {
    if (uniform_locations.find(name) != uniform_locations.end()) {
        return uniform_locations[name];
    }

    GLint location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        throw Exception("Uniform '" + name + "' not found or invalid.");
    }

    uniform_locations[name] = location;
    return location;
}

// ========================================
//      SINGLE UNIFORMS
// ========================================
template<class T>
void Shader::set_uniform_vec1(const std::string& name, const T& v0) {
    bind();
    GLuint location = get_uniform_location(name);

    if constexpr (std::is_same_v<T, int> || std::is_same_v<T, GLint>) {
        glUniform1i(location, v0);
    }
    else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, GLfloat>) {
        glUniform1f(location, v0);
    }
    else if constexpr (std::is_same_v<T, GLuint> || std::is_same_v<T, unsigned int>) {
        glUniform1ui(location, v0);
    }
    else {
        throw Exception("Unsupported type for set_uniform_vec1.");
    }
}

// ========================================
//      VEC2 UNIFORMS
// ========================================
template<class T>
void Shader::set_uniform_vec2(const std::string& name, const T& v0, const T& v1) {
    bind();
    GLuint location = get_uniform_location(name);

    if constexpr (std::is_same_v<T, int> || std::is_same_v<T, GLint>) {
        glUniform2i(location, v0, v1);
    }
    else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, GLfloat>) {
        glUniform2f(location, v0, v1);
    }
    else if constexpr (std::is_same_v<T, GLuint> || std::is_same_v<T, unsigned int>) {
        glUniform2ui(location, v0, v1);
    }
    else {
        throw Exception("Unsupported type for set_uniform_vec2.");
    }
}

// ========================================
//      VEC3 UNIFORMS
// ========================================
template<class T>
void Shader::set_uniform_vec3(const std::string& name, const T& v0, const T& v1, const T& v2) {
    bind();
    GLuint location = get_uniform_location(name);

    if constexpr (std::is_same_v<T, int> || std::is_same_v<T, GLint>) {
        glUniform3i(location, v0, v1, v2);
    }
    else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, GLfloat>) {
        glUniform3f(location, v0, v1, v2);
    }
    else if constexpr (std::is_same_v<T, GLuint> || std::is_same_v<T, unsigned int>) {
        glUniform3ui(location, v0, v1, v2);
    }
    else {
        throw Exception("Unsupported type for set_uniform_vec3.");
    }
}

// ========================================
//      VEC4 UNIFORMS
// ========================================
template<class T>
void Shader::set_uniform_vec4(const std::string& name, const T& v0, const T& v1, const T& v2, const T& v3) {
    bind();
    GLuint location = get_uniform_location(name);

    if constexpr (std::is_same_v<T, int> || std::is_same_v<T, GLint>) {
        glUniform4i(location, v0, v1, v2, v3);
    }
    else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, GLfloat>) {
        glUniform4f(location, v0, v1, v2, v3);
    }
    else if constexpr (std::is_same_v<T, GLuint> || std::is_same_v<T, unsigned int>) {
        glUniform4ui(location, v0, v1, v2, v3);
    }
    else {
        throw Exception("Unsupported type for set_uniform_vec4.");
    }
}

// ========================================
//      MATRIX UNIFORMS
// ========================================
template<class T>
void Shader::set_uniform_matrix(const std::string& name, const T& matrix) {
    bind();
    GLuint location = get_uniform_location(name);

    if constexpr (std::is_same_v<T, glm::mat2>) {
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    else if constexpr (std::is_same_v<T, glm::mat3>) {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    else if constexpr (std::is_same_v<T, glm::mat4>) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    else if constexpr (std::is_same_v<T, glm::mat2x3>) {
        glUniformMatrix2x3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    else if constexpr (std::is_same_v<T, glm::mat3x2>) {
        glUniformMatrix3x2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    else if constexpr (std::is_same_v<T, glm::mat2x4>) {
        glUniformMatrix2x4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    else if constexpr (std::is_same_v<T, glm::mat4x2>) {
        glUniformMatrix4x2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    else if constexpr (std::is_same_v<T, glm::mat3x4>) {
        glUniformMatrix3x4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    else if constexpr (std::is_same_v<T, glm::mat4x3>) {
        glUniformMatrix4x3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    else {
        throw Exception("Unsupported type for set_uniform_matrix.");
    }
}

// ========================================
//      MATRIX ARRAY UNIFORMS
// ========================================
template<class T>
void Shader::set_uniform_matrix_array(const std::string& name, int count, const T* matrices) {
    bind();
    GLuint location = get_uniform_location(name);

    if constexpr (std::is_same_v<T, glm::mat2>) {
        glUniformMatrix2fv(location, count, GL_FALSE, glm::value_ptr(matrices[0]));
    }
    else if constexpr (std::is_same_v<T, glm::mat3>) {
        glUniformMatrix3fv(location, count, GL_FALSE, glm::value_ptr(matrices[0]));
    }
    else if constexpr (std::is_same_v<T, glm::mat4>) {
        glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(matrices[0]));
    }
    else if constexpr (std::is_same_v<T, glm::mat2x3>) {
        glUniformMatrix2x3fv(location, count, GL_FALSE, glm::value_ptr(matrices[0]));
    }
    else if constexpr (std::is_same_v<T, glm::mat3x2>) {
        glUniformMatrix3x2fv(location, count, GL_FALSE, glm::value_ptr(matrices[0]));
    }
    else if constexpr (std::is_same_v<T, glm::mat2x4>) {
        glUniformMatrix2x4fv(location, count, GL_FALSE, glm::value_ptr(matrices[0]));
    }
    else if constexpr (std::is_same_v<T, glm::mat4x2>) {
        glUniformMatrix4x2fv(location, count, GL_FALSE, glm::value_ptr(matrices[0]));
    }
    else if constexpr (std::is_same_v<T, glm::mat3x4>) {
        glUniformMatrix3x4fv(location, count, GL_FALSE, glm::value_ptr(matrices[0]));
    }
    else if constexpr (std::is_same_v<T, glm::mat4x3>) {
        glUniformMatrix4x3fv(location, count, GL_FALSE, glm::value_ptr(matrices[0]));
    }
    else {
        throw Exception("Unsupported type for set_uniform_matrix_array.");
    }
}

// ============================
// EXPLICIT TEMPLATE INSTANTIATION
// ============================

template void Shader::set_uniform_matrix<glm::mat2>(const std::string&, const glm::mat2&);
template void Shader::set_uniform_matrix<glm::mat3>(const std::string&, const glm::mat3&);
template void Shader::set_uniform_matrix<glm::mat4>(const std::string&, const glm::mat4&);
template void Shader::set_uniform_matrix<glm::mat2x3>(const std::string&, const glm::mat2x3&);
template void Shader::set_uniform_matrix<glm::mat3x2>(const std::string&, const glm::mat3x2&);
template void Shader::set_uniform_matrix<glm::mat2x4>(const std::string&, const glm::mat2x4&);
template void Shader::set_uniform_matrix<glm::mat4x2>(const std::string&, const glm::mat4x2&);
template void Shader::set_uniform_matrix<glm::mat3x4>(const std::string&, const glm::mat3x4&);
template void Shader::set_uniform_matrix<glm::mat4x3>(const std::string&, const glm::mat4x3&);

}
