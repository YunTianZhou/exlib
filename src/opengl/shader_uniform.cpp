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

    if constexpr (std::is_same_v<T, GLint>) {
        glUniform1i(location, v0);
    }
    else if constexpr (std::is_same_v<T, GLfloat>) {
        glUniform1f(location, v0);
    }
    else if constexpr (std::is_same_v<T, GLuint>) {
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

    if constexpr (std::is_same_v<T, GLint>) {
        glUniform2i(location, v0, v1);
    }
    else if constexpr (std::is_same_v<T, GLfloat>) {
        glUniform2f(location, v0, v1);
    }
    else if constexpr (std::is_same_v<T, GLuint>) {
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

    if constexpr (std::is_same_v<T, GLint>) {
        glUniform3i(location, v0, v1, v2);
    }
    else if constexpr (std::is_same_v<T, GLfloat>) {
        glUniform3f(location, v0, v1, v2);
    }
    else if constexpr (std::is_same_v<T, GLuint>) {
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

    if constexpr (std::is_same_v<T, GLint>) {
        glUniform4i(location, v0, v1, v2, v3);
    }
    else if constexpr (std::is_same_v<T, GLfloat>) {
        glUniform4f(location, v0, v1, v2, v3);
    }
    else if constexpr (std::is_same_v<T, GLuint>) {
        glUniform4ui(location, v0, v1, v2, v3);
    }
    else {
        throw Exception("Unsupported type for set_uniform_vec4.");
    }
}

// ========================================
//      VEC1 ARRAY UNIFORMS
// ========================================
template <class T>
void Shader::set_uniform_vec1_array(const std::string& name, GLsizei count, const T* value) {
    bind();
    GLuint location = get_uniform_location(name);

    if constexpr (std::is_same_v<T, GLint>) {
        glUniform1iv(location, count, value);
    }
    else if constexpr (std::is_same_v<T, GLfloat>) {
        glUniform1fv(location, count, value);
    }
    else if constexpr (std::is_same_v<T, GLuint>) {
        glUniform1uiv(location, count, value);
    }
    else {
        throw Exception("Unsupported type for set_uniform_vec1_array.");
    }
}

// ========================================
//      VEC2 ARRAY UNIFORMS
// ========================================
template <class T>
void Shader::set_uniform_vec2_array(const std::string& name, GLsizei count, const T* value) {
    bind();
    GLuint location = get_uniform_location(name);

    if constexpr (std::is_same_v<T, GLint>) {
        glUniform2iv(location, count, value);
    }
    else if constexpr (std::is_same_v<T, GLfloat>) {
        glUniform2fv(location, count, value);
    }
    else if constexpr (std::is_same_v<T, GLuint>) {
        glUniform2uiv(location, count, value);
    }
    else {
        throw Exception("Unsupported type for set_uniform_vec2_array.");
    }
}

// ========================================
//      VEC3 ARRAY UNIFORMS
// ========================================
template <class T>
void Shader::set_uniform_vec3_array(const std::string& name, GLsizei count, const T* value) {
    bind();
    GLuint location = get_uniform_location(name);

    if constexpr (std::is_same_v<T, GLint>) {
        glUniform3iv(location, count, value);
    }
    else if constexpr (std::is_same_v<T, GLfloat>) {
        glUniform3fv(location, count, value);
    }
    else if constexpr (std::is_same_v<T, GLuint>) {
        glUniform3uiv(location, count, value);
    }
    else {
        throw Exception("Unsupported type for set_uniform_vec3_array.");
    }
}

// ========================================
//      VEC4 ARRAY UNIFORMS
// ========================================
template <class T>
void Shader::set_uniform_vec4_array(const std::string& name, GLsizei count, const T* value) {
    bind();
    GLuint location = get_uniform_location(name);

    if constexpr (std::is_same_v<T, GLint>) {
        glUniform4iv(location, count, value);
    }
    else if constexpr (std::is_same_v<T, GLfloat>) {
        glUniform4fv(location, count, value);
    }
    else if constexpr (std::is_same_v<T, GLuint>) {
        glUniform4uiv(location, count, value);
    }
    else {
        throw Exception("Unsupported type for set_uniform_vec4_array.");
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
void Shader::set_uniform_matrix_array(const std::string& name, GLsizei count, const T* matrices) {
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

template void Shader::set_uniform_vec1<GLint>(const std::string&, const GLint&);
template void Shader::set_uniform_vec1<GLfloat>(const std::string&, const GLfloat&);
template void Shader::set_uniform_vec1<GLuint>(const std::string&, const GLuint&);

template void Shader::set_uniform_vec2<GLint>(const std::string&, const GLint&, const GLint&);
template void Shader::set_uniform_vec2<GLfloat>(const std::string&, const GLfloat&, const GLfloat&);
template void Shader::set_uniform_vec2<GLuint>(const std::string&, const GLuint&, const GLuint&);

template void Shader::set_uniform_vec3<GLint>(const std::string&, const GLint&, const GLint&, const GLint&);
template void Shader::set_uniform_vec3<GLfloat>(const std::string&, const GLfloat&, const GLfloat&, const GLfloat&);
template void Shader::set_uniform_vec3<GLuint>(const std::string&, const GLuint&, const GLuint&, const GLuint&);

template void Shader::set_uniform_vec4<GLint>(const std::string&, const GLint&, const GLint&, const GLint&, const GLint&);
template void Shader::set_uniform_vec4<GLfloat>(const std::string&, const GLfloat&, const GLfloat&, const GLfloat&, const GLfloat&);
template void Shader::set_uniform_vec4<GLuint>(const std::string&, const GLuint&, const GLuint&, const GLuint&, const GLuint&);

template void Shader::set_uniform_vec1_array<GLint>(const std::string&, GLsizei, const GLint*);
template void Shader::set_uniform_vec1_array<GLfloat>(const std::string&, GLsizei, const GLfloat*);
template void Shader::set_uniform_vec1_array<GLuint>(const std::string&, GLsizei, const GLuint*);

template void Shader::set_uniform_vec2_array<GLint>(const std::string&, GLsizei, const GLint*);
template void Shader::set_uniform_vec2_array<GLfloat>(const std::string&, GLsizei, const GLfloat*);
template void Shader::set_uniform_vec2_array<GLuint>(const std::string&, GLsizei, const GLuint*);

template void Shader::set_uniform_vec3_array<GLint>(const std::string&, GLsizei, const GLint*);
template void Shader::set_uniform_vec3_array<GLfloat>(const std::string&, GLsizei, const GLfloat*);
template void Shader::set_uniform_vec3_array<GLuint>(const std::string&, GLsizei, const GLuint*);

template void Shader::set_uniform_vec4_array<GLint>(const std::string&, GLsizei, const GLint*);
template void Shader::set_uniform_vec4_array<GLfloat>(const std::string&, GLsizei, const GLfloat*);
template void Shader::set_uniform_vec4_array<GLuint>(const std::string&, GLsizei, const GLuint*);

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
