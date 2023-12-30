#include "hzpch.h"
#include "Sandbox2D.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

double M_PI = glm::pi<double>();

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(16.0f / 9.0f, true)
{
}

Sandbox2D::~Sandbox2D()
{
	HZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_Texture = Hazel::Texture2D::Create("assets/textures/checkerboard.png");
	m_SpriteSheet = Hazel::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

	// Background Quad
	m_QuadData = Hazel::QuadData3DBuilder()
		.SetPosition({ 0.0f, 0.0f, -0.01f })
		.SetScale({ 40.0f, 40.0f, 1.0f })
		.SetTint(m_BackgroundTintColor)
		.SetTexture(m_Texture)
		.SetTilingFactor(20.0f);

	// Rotating Quad
	m_RotatingQuadData = Hazel::QuadData3DBuilder()
		.SetPosition({ -2.0f, -0.5f, 0.01f })
		.SetScale({ 1.5f, 0.5f, 1.0f })
		.SetColor({ 0.8f, 0.2f, 0.3f, 0.8f });

	// Square
	m_SquareData = Hazel::QuadData3DBuilder()
		.SetPosition({ 0.0f, 0.0f, 0.02f })
		.SetScale({ 1.0f, 1.0f, 1.0f })
		.SetColor(m_SquareColor);

	m_QuadBuilder2D = Hazel::CreateRef<Hazel::QuadData2DBuilder>(Hazel::QuadData2DBuilder());
	m_QuadBuilder3D = Hazel::CreateRef<Hazel::QuadData3DBuilder>(Hazel::QuadData3DBuilder());

	m_Particle.ColorBegin = { 0.8f, 0.8f, 0.2f, 1.0f };
	m_Particle.ColorEnd = { 0.2f, 0.2f, 0.8f, 0.0f };
	m_Particle.SizeBegin = 0.25f;
	m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 5.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
	m_Particle.SizeVariation = 0.3f;
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	m_Timestep = 1 / ts;
	HZ_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);


	Hazel::Renderer2D::ResetStats();

	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor({ 0.09020f, 0.10196f, 0.12157f, 1 });
		Hazel::RenderCommand::Clear();
	}

#if 1
	{
		HZ_PROFILE_SCOPE("Renderer Draw");

		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		m_RotatingQuadData->transform->rotation += m_QuadRotationSpeed * ts;

		static float scale = 0.1f;
		for (float y = -5.0f; y < 5.0f; y += scale) {
			for (float x = -5.0f; x < 5.0f; x += scale) {
				float angle = atan2(y, x);
				double hue = (angle / (2 * M_PI)) * 360.0f;
				hue = fmod(hue + (m_RotatingQuadData->transform->rotation * 180 / M_PI), 360.0f);
				auto color = HSVtoRGB(hue, 0.5f, 0.85f);

				Hazel::Renderer2D::DrawQuad(m_QuadBuilder2D->SetPosition({ x, y }) // z = 0.0f
					.SetScale({ scale , scale })
					.SetColor({ color.r, color.g, color.b, 1.0f }));
			}
		}

		Hazel::Renderer2D::EndScene();
	}
#endif

	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Hazel::Input::GetMousePosition();
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Hazel::Renderer2D::DrawQuad(m_QuadBuilder3D->SetPosition({ 0.0f, 0.0f, 0.5f })
		.SetScale({ 1.0f , 1.0f, 1.0f })
		.SetTexture(m_SpriteSheet));
	Hazel::Renderer2D::EndScene();
}

glm::vec3 Sandbox2D::HSVtoRGB(double h, float s, float v) {
	float c = v * s;
	float x = (float)(c * (1 - abs(fmod(h / 60.0f, 2) - 1)));
	float m = v - c;
	float r, g, b;
	if (h >= 0 && h < 60) {
		r = c;
		g = x;
		b = 0;
	}
	else if (h >= 60 && h < 120) {
		r = x;
		g = c;
		b = 0;
	}
	else if (h >= 120 && h < 180) {
		r = 0;
		g = c;
		b = x;
	}
	else if (h >= 180 && h < 240) {
		r = 0;
		g = x;
		b = c;
	}
	else if (h >= 240 && h < 300) {
		r = x;
		g = 0;
		b = c;
	}
	else {
		r = c;
		g = 0;
		b = x;
	}
	return { r + m, g + m, b + m };
}

void Sandbox2D::HandleInput(Hazel::Timestep ts) {
	if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
		m_RotatingQuadData->transform->rotation -= m_QuadRotationSpeed * ts;
	if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
		m_RotatingQuadData->transform->rotation += m_QuadRotationSpeed * ts;
	m_RotatingQuadData->transform->rotation += m_RotatingQuadData->transform->rotation >= m_QuadRotationSpeed ?
		-m_QuadRotationSpeed :
		m_RotatingQuadData->transform->rotation < 0 ?
		m_QuadRotationSpeed : 0.0f;
}

void Sandbox2D::OnImGuiRender() {
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Application Stats:");
	ImGui::Indent();
	ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000.0f / m_Timestep, m_Timestep);
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Renderer2D Stats:");
	ImGui::Indent();
	ImGui::Text("Draw Calls: %d", Hazel::Renderer2D::GetStats().DrawCalls);
	ImGui::Text("Quad Count: %d", Hazel::Renderer2D::GetStats().QuadCount);
	ImGui::Text("Vertex Count: %d", Hazel::Renderer2D::GetStats().GetTotalVertexCount());
	ImGui::Text("Index Count: %d", Hazel::Renderer2D::GetStats().GetTotalIndexCount());
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::ColorEdit4("Square Tint", glm::value_ptr(m_BackgroundTintColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
