#include <Hazel.h>

class DemoLayer : public Hazel::Layer {
public:
	DemoLayer()
		: Hazel::Layer("Demo"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		////////////////////////////////////////////
		// Triangle ///////////////////////////////
		//////////////////////////////////////////
		m_TriangleVA.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.3f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.8f, 0.5f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.7f, 0.2f, 0.65f, 1.0f
		};
		std::shared_ptr<Hazel::VertexBuffer> triangleVB;
		triangleVB.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			triangleVB->SetLayout({
				{ Hazel::ShaderDataType::Float3, "a_Position" },
				{ Hazel::ShaderDataType::Float4, "a_Color"}
				});

			m_TriangleVA->AddVertexBuffer(triangleVB);
		}

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Hazel::IndexBuffer> triangleIB;
		triangleIB.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_TriangleVA->SetIndexBuffer(triangleIB);

		std::string triangleVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			
			out vec3 v_Position;
			out vec4 v_Color;
			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string triangleFragmentSrc = R"(
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

		m_TriangleShader.reset(new Hazel::Shader(triangleVertexSrc, triangleFragmentSrc));

		////////////////////////////////////////////
		// Square /////////////////////////////////
		//////////////////////////////////////////
		m_SquareVA.reset(Hazel::VertexArray::Create());
		float squareVertices[4 * 3] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		{
			squareVB->SetLayout({
				{ Hazel::ShaderDataType::Float3, "a_Position" }
				});
			m_SquareVA->AddVertexBuffer(squareVB);
		}

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string squareFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		m_BackgroundShader.reset(new Hazel::Shader(squareVertexSrc, squareFragmentSrc));

	}

	void OnUpdate(Hazel::Timestep ts) override {
		HZ_TRACE("{0}", ts);
		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;


		Hazel::RenderCommand::SetClearColor({ 0.09020f, 0.10196f, 0.12157f, 1 });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);


		Hazel::Renderer::BeginScene(m_Camera);

		Hazel::Renderer::Submit(m_BackgroundShader, m_SquareVA);
		Hazel::Renderer::Submit(m_TriangleShader, m_TriangleVA);

		Hazel::Renderer::EndScene();
	}
private:
	std::shared_ptr<Hazel::Shader> m_BackgroundShader;
	std::shared_ptr<Hazel::Shader> m_TriangleShader;

	std::shared_ptr<Hazel::VertexArray> m_SquareVA;
	std::shared_ptr<Hazel::VertexArray> m_TriangleVA;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;
};


class Sandbox : public Hazel::Application {
public:
	Sandbox()
	{
		PushLayer(new DemoLayer());
	}
	~Sandbox() { }
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}