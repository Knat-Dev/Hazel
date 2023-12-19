#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include "Sandbox2D.h"

class DemoLayer : public Hazel::Layer {
public:
	DemoLayer()
		: Hazel::Layer("Demo"), m_SquarePosition(0.0f), m_CameraController(16.0f / 9.0f, true)
	{
		////////////////////////////////////////////
		// Triangle ///////////////////////////////
		//////////////////////////////////////////
		m_TriangleVA = Hazel::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.3f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.8f, 0.5f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.7f, 0.2f, 0.65f, 1.0f
		};
		Hazel::Ref<Hazel::VertexBuffer> triangleVB;
		triangleVB = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));

		{
			triangleVB->SetLayout({
				{ Hazel::ShaderDataType::Float3, "a_Position" },
				{ Hazel::ShaderDataType::Float4, "a_Color"}
				});

			m_TriangleVA->AddVertexBuffer(triangleVB);
		}

		uint32_t indices[3] = { 0, 1, 2 };
		Hazel::Ref<Hazel::IndexBuffer> triangleIB;
		triangleIB = Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_TriangleVA->SetIndexBuffer(triangleIB);

		auto triangleShader = m_ShaderLibrary.Load("assets/shaders/TriangleShader.glsl");

		////////////////////////////////////////////
		// Square /////////////////////////////////
		//////////////////////////////////////////
		m_SquareVA = Hazel::VertexArray::Create();
		float squareVertices[4 * (3 + 2)] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f ,0.0f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		{
			squareVB->SetLayout({
				{ Hazel::ShaderDataType::Float3, "a_Position" },
				{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
				});
			m_SquareVA->AddVertexBuffer(squareVB);
		}

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB = Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		// Shaders
		// 
		// Flat color shader
		auto flatColorShader = m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
		// Texture shader
		m_MinecraftTexture = Hazel::Texture2D::Create("assets/textures/minecraft.jpg");
		m_CppLogoTexture = Hazel::Texture2D::Create("assets/textures/cpp.png");
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		textureShader->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazel::Timestep ts) override {
		m_CameraController.OnUpdate(ts);

		HandleKeyboardInput(ts);

		Hazel::RenderCommand::SetClearColor({ 0.09020f, 0.10196f, 0.12157f, 1 });
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());


		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto flatColorShader = m_ShaderLibrary.Get("FlatColor");
		flatColorShader->Bind();

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++)
			{
				std::dynamic_pointer_cast<Hazel::OpenGLShader>(flatColorShader)->UploadUniformFloat4("u_Color",
					(i + j) % 2 == 0 ? m_CheckerboardSquareColor1 : m_CheckerboardSquareColor2);


				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hazel::Renderer::Submit(flatColorShader, m_SquareVA, transform);
			}
		}

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(flatColorShader)->UploadUniformFloat4("u_Color", m_CheckerboardSquareColor1);


		glm::mat4 largeScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition) * largeScale;

		auto textureShader = m_ShaderLibrary.Get("Texture");
		m_MinecraftTexture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVA, transform);
		m_CppLogoTexture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVA, transform);

		//auto triangleShader = m_ShaderLibrary.Get("TriangleShader");
		//Hazel::Renderer::Submit(triangleShader, m_TriangleVA);

		Hazel::Renderer::EndScene();
	}

	void OnEvent(Hazel::Event& e) override {
		m_CameraController.OnEvent(e);
	}

	void OnImGuiRender() override {
		ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::ColorEdit4("Checkerboard Color 1", glm::value_ptr(m_CheckerboardSquareColor1));
		ImGui::ColorEdit4("Checkerboard Color 2", glm::value_ptr(m_CheckerboardSquareColor2));
		ImGui::End();
	}

	void HandleKeyboardInput(Hazel::Timestep ts) {
		if (Hazel::Input::IsKeyPressed(HZ_KEY_I))
			m_SquarePosition.y += m_SquareMoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_K))
			m_SquarePosition.y -= m_SquareMoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_J))
			m_SquarePosition.x -= m_SquareMoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_L))
			m_SquarePosition.x += m_SquareMoveSpeed * ts;
	}
private:
	Hazel::ShaderLibrary m_ShaderLibrary;

	Hazel::Ref<Hazel::Texture2D> m_MinecraftTexture, m_CppLogoTexture;

	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::VertexArray> m_TriangleVA;

	Hazel::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 1.0f;

	// Checkerboard
	glm::vec4 m_CheckerboardSquareColor1 = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_CheckerboardSquareColor2 = { 0.3f, 0.2f, 0.8f, 1.0f };
};


class Sandbox : public Hazel::Application {
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}
	~Sandbox() { }
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}