#pragma once

#include <GL/glew.h>
#include "exlib/core/types.hpp"

namespace ex::gl {

class VertexArray;
class IndexBuffer;
class Shader;

class EXLIB_API Render {
public:
	enum class Mode {
		Points = GL_POINTS,
		LineStrip = GL_LINE_STRIP,
		LineLoop = GL_LINE_LOOP,
		Lines = GL_LINES,
		LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
		LinesAdjacency = GL_LINES_ADJACENCY,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN,
		Triangles = GL_TRIANGLES,
		TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
		TrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
		Patches = GL_PATCHES
	};

public:
	Render() = delete;

	static void clear();
	static void set_clear_color(Color color);
	static void set_viewport(Vec2i pos, Vec2i size);

	static void draw_arrays(Mode mode, const VertexArray& vao, const Shader& shader, GLint first = 0, GLsizei count = -1);
	static void draw_elements(Mode mode, const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader, GLint first = 0, GLsizei count = -1);

};

}