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

	struct QuadData3D
	{
		Ref<Transform<glm::vec3>> transform = CreateRef<Transform<glm::vec3>>();
		glm::vec4 color = glm::vec4(1.0f);
		Ref<Texture2D> texture = nullptr;
		float tilingFactor = 1.0f;
		glm::vec4 tint = glm::vec4(1.0f);

		QuadData3D() = default;
	};

	class QuadData2DBuilder
	{
	public:
		QuadData2DBuilder()
			: m_Data(CreateRef<QuadData2D>())
		{
		}

		QuadData2DBuilder& SetPosition(const glm::vec2& position)
		{
			m_Data->transform->position = position;
			return *this;
		}

		QuadData2DBuilder& SetPosition(float x, float y)
		{
			m_Data->transform->position = { x, y };
			return *this;
		}

		QuadData2DBuilder& SetScale(const glm::vec2& scale)
		{
			m_Data->transform->scale = scale;
			return *this;
		}

		QuadData2DBuilder& SetScale(float x, float y)
		{
			m_Data->transform->scale = { x, y };
			return *this;
		}

		QuadData2DBuilder& SetRotation(float rotation)
		{
			m_Data->transform->rotation = rotation;
			return *this;
		}

		QuadData2DBuilder& SetColor(const glm::vec4& color)
		{
			m_Data->color = color;
			return *this;
		}

		QuadData2DBuilder& SetTexture(const Ref<Texture2D>& texture)
		{
			m_Data->texture = texture;
			return *this;
		}

		QuadData2DBuilder& SetTilingFactor(float tilingFactor)
		{
			m_Data->tilingFactor = tilingFactor;
			return *this;
		}

		QuadData2DBuilder& SetTint(const glm::vec4& tint)
		{
			m_Data->tint = tint;
			return *this;
		}

		operator Ref<QuadData2D>() { return m_Data; }

	private:
		Ref<QuadData2D> m_Data;
	};

	class QuadData3DBuilder
	{
	public:
		QuadData3DBuilder()
			: m_Data(CreateRef<QuadData3D>())
		{
		}

		QuadData3DBuilder& SetPosition(const glm::vec3& position)
		{
			m_Data->transform->position = position;
			return *this;
		}

		QuadData3DBuilder& SetPosition(float x, float y, float z)
		{
			m_Data->transform->position = { x, y, z };
			return *this;
		}

		QuadData3DBuilder& SetScale(const glm::vec3& scale)
		{
			m_Data->transform->scale = scale;
			return *this;
		}

		QuadData3DBuilder& SetScale(float x, float y, float z)
		{
			m_Data->transform->scale = { x, y, z };
			return *this;
		}

		QuadData3DBuilder& SetRotation(float rotation)
		{
			m_Data->transform->rotation = rotation;
			return *this;
		}

		QuadData3DBuilder& SetColor(const glm::vec4& color)
		{
			m_Data->color = color;
			return *this;
		}

		QuadData3DBuilder& SetTexture(const Ref<Texture2D>& texture)
		{
			m_Data->texture = texture;
			return *this;
		}

		QuadData3DBuilder& SetTilingFactor(float tilingFactor)
		{
			m_Data->tilingFactor = tilingFactor;
			return *this;
		}

		QuadData3DBuilder& SetTint(const glm::vec4& tint)
		{
			m_Data->tint = tint;
			return *this;
		}

		operator Ref<QuadData3D>() { return m_Data; }

	private:
		Ref<QuadData3D> m_Data;
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
		static void DrawQuad(const Ref<QuadData3D>& data);
	};
}
