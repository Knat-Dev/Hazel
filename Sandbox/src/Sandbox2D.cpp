#include "hzpch.h"
#include "Sandbox2D.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(16.0f / 9.0f, true)
{
	m_Texture = Hazel::Texture2D::Create("assets/textures/checkerboard.png");
}

void Sandbox2D::OnAttach()
{
	
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	HandleInput(ts);

	Hazel::RenderCommand::SetClearColor({ 0.09020f, 0.10196f, 0.12157f, 1 });
	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f}, { 0.8f, 0.8f }, 0.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f}, { 0.5f, 0.75f }, m_QuadRotation, m_SquareColor);
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, 0.0f, m_Texture);
	Hazel::Renderer2D::EndScene();
}

void Sandbox2D::HandleInput(Hazel::Timestep ts) {
	if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
		m_QuadRotation -= 360.0f * ts;
	if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
		m_QuadRotation += 360.0f * ts;
	m_QuadRotation += m_QuadRotation >= 360.0f ? -360.0f : m_QuadRotation < 0 ? 360.0f : 0.0f;
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
