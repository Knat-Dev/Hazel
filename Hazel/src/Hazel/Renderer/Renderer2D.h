#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"

namespace Hazel {
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		// With color
		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const glm::vec4& color);
		// With texture
		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const Ref<Texture2D>& texture);
	};
}
