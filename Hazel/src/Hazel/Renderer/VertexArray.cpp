#include "hzpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			break;
		case RendererAPI::OpenGL:
			return new OpenGLVertexArray();
			break;
		default:
			break;
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
	}

	void VertexArray::Bind() const
	{
	}

	void VertexArray::Unbind() const
	{
	}
}