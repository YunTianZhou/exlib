#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include "exlib/core/config.hpp"

namespace ex::gl {

class EXLIB_API Shader {
public:
	struct ProgramSource {
		std::string vertex;
		std::string fragment;

        /*
            Read the shader source from a file with the following format:
            ____________________________________________________________

            #shader vertex

            ...( vertex shader source )

            #shader fragment

            ...( fragment shader source )

            ____________________________________________________________
        */
        static ProgramSource ReadFromFile(const std::string& filepath);

        static ProgramSource ReadFromFile(const std::string& vertex, const std::string& fragment);
	};

public:
    // Constructor and destructor
    explicit Shader(const ProgramSource& source);
    ~Shader();

    // Copy and Move
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other);
    Shader& operator=(Shader&& other);

    // Binding and unbinding
    inline void bind() const { glUseProgram(id); }
    inline void unbind() const { glUseProgram(0); }

public:
    // Uniform Setters
    template <class T> void set_uniform_vec1(const std::string& name, const T& v0);
    template <class T> void set_uniform_vec2(const std::string& name, const T& v0, const T& v1);
    template <class T> void set_uniform_vec3(const std::string& name, const T& v0, const T& v1, const T& v2);
    template <class T> void set_uniform_vec4(const std::string& name, const T& v0, const T& v1, const T& v2, const T& v3);
    template <class T> void set_uniform_vec1_array(const std::string& name, GLsizei count, const T* value);
    template <class T> void set_uniform_vec2_array(const std::string& name, GLsizei count, const T* value);
    template <class T> void set_uniform_vec3_array(const std::string& name, GLsizei count, const T* value);
    template <class T> void set_uniform_vec4_array(const std::string& name, GLsizei count, const T* value);
    template <class T> void set_uniform_matrix(const std::string& name, const T& matrix);
    template <class T> void set_uniform_matrix_array(const std::string& name, GLsizei count, const T* matrices);

private:
    GLuint get_uniform_location(const std::string& name);

private:
    GLuint create_shader(const ProgramSource& source) const;
    GLuint compile_shader(GLenum type, const std::string& source) const;

private:
    GLuint id;
    std::unordered_map<std::string, GLuint> uniform_locations;

};

}
