#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"

namespace Hazel {

	template<typename T>
	struct Transform {
		T position = T(0.0f);
		float rotation = 0.0f;
		T scale = T(1.0f);

		Transform() = default;
	};

	struct QuadData2D
	{
		Ref<Transform<glm::vec2>> transform = CreateRef<Transform<glm::vec2>>();
		glm::vec4 color = glm::vec4(1.0f);
		Ref<Texture2D> texture = nullptr;
		float tilingFactor = 1.0f;
		glm::vec4 tint = glm::vec4(1.0f);

		QuadData2D() = default;
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		// With color
		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
		// With texture														  
		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);

		// Rotated primitives
		// With color
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const glm::vec4& color);
		// With texture
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);

		// with struct
		static void DrawQuad(const Ref<QuadData2D>& data);
	};
}
