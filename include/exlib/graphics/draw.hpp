#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "exlib/graphics/drawable.hpp"
#include "exlib/graphics/types.hpp"
#include "exlib/opengl/types.hpp"

namespace ex {

class Texture;

namespace gl {
	class VertexBuffer;
	class VertexArray;
	class Shader;
}

class EXLIB_API Draw {
public:
	struct State {
		PrimitiveType type;
		const glm::mat4* transform = nullptr;
		const Texture* texture = nullptr;

		State() = default;
		State(PrimitiveType type, const glm::mat4* transform = nullptr, const Texture* texture = nullptr)
			: type(type), transform(transform), texture(texture) {}
	};

public:
	Draw() = delete;

	static void draw(const std::vector<Vertex>& vertices, const State& state);
	static void draw(const Vertex* start, int count, const State& state);
	static void draw(const Drawable& drawable);

private:
	static void init_color_pipeline();
	static void init_texture_pipeline();

	static glm::mat4 get_ortho_transform(const glm::mat4* _transform);
	static void draw_color(const Vertex* start, int count, const State& state);
	static void draw_texture(const Vertex* start, int count, const State& state);

private:
	static std::unique_ptr<gl::VertexBuffer> color_vbo;
	static std::unique_ptr<gl::VertexArray> color_vao;
	static std::unique_ptr<gl::Shader> color_shader;

	static std::unique_ptr<gl::VertexBuffer> texture_vbo;
	static std::unique_ptr<gl::VertexArray> texture_vao;
	static std::unique_ptr<gl::Shader> texture_shader;
};

}