#include <Hazel.h>

#include "imgui/imgui.h"

using namespace Hazel;

class ExampleLayer : public Layer {
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnAttach() override {
		m_TriangleVA.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.3f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.8f, 0.5f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.7f, 0.2f, 0.65f, 1.0f
		};
		std::shared_ptr<VertexBuffer> triangleVB;
		triangleVB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			triangleVB->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color"}
				});

			m_TriangleVA->AddVertexBuffer(triangleVB);
		}

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> triangleIB;
		triangleIB.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_TriangleVA->SetIndexBuffer(triangleIB);

		// Square Vertex Array
		m_SquareVA.reset(VertexArray::Create());
		float squareVertices[4 * 3] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		{
			squareVB->SetLayout({
				{ ShaderDataType::Float3, "a_Position" }
				});
			m_SquareVA->AddVertexBuffer(squareVB);
		}

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		// Static Shaders
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			out vec3 v_Position;
			out vec4 v_Color;
			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_TriangleShader.reset(new Shader(vertexSrc, fragmentSrc));

		std::string vertexSrc2 = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			
			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		m_BackgroundShader.reset(new Shader(vertexSrc2, fragmentSrc2));
	}

	void OnUpdate() override {
		RenderCommand::SetClearColor({ 0.09020f, 0.10196f, 0.12157f, 1 });
		RenderCommand::Clear();

		Renderer::BeginScene();

		m_BackgroundShader->Bind();
		Renderer::Submit(m_SquareVA);

		m_TriangleShader->Bind();
		Renderer::Submit(m_TriangleVA);

		Renderer::EndScene();
	}

	void OnEvent(Event& event) override {
		if (event.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& e = (KeyPressedEvent&)event;
			if (e.GetKeyCode() == HZ_KEY_TAB)
				HZ_TRACE("Tab key is pressed (event)!");
			HZ_TRACE("{0}", (char)e.GetKeyCode());

		}
	}

private:
	std::shared_ptr<Shader> m_BackgroundShader;
	std::shared_ptr<Shader> m_TriangleShader;

	std::shared_ptr<VertexArray> m_SquareVA;
	std::shared_ptr<VertexArray> m_TriangleVA;
};


class Sandbox : public Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() { }
};

Application* Hazel::CreateApplication() {
	return new Sandbox();
}