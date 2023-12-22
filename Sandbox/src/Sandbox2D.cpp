#include "hzpch.h"
#include "Sandbox2D.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

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
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	{
		HZ_PROFILE_SCOPE("Sandbox2D::HandleInput");
		HandleInput(ts);
	}

	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor({ 0.09020f, 0.10196f, 0.12157f, 1 });
		Hazel::RenderCommand::Clear();
	}

	{
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, 0.0f, m_Texture);
		Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.5f }, m_QuadRotation, m_SquareColor);
		Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 1.6f }, 0.0f, { 0.8f, 0.2f, 0.3f, 0.8f });
		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::HandleInput(Hazel::Timestep ts) {
	if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
		m_QuadRotation -= 360.0f * ts;
	if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
		m_QuadRotation += 360.0f * ts;
	m_QuadRotation += m_QuadRotation >= 360.0f ? -360.0f : m_QuadRotation < 0 ? 360.0f : 0.0f;
}

void Sandbox2D::OnImGuiRender() {
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	auto flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit;

	ImGui::Text("Profile Results"); // This is your header title, make it bold or style as you like.
	ImGui::Spacing();

	if (ImGui::BeginTable("ProfileResultsTable", 2, flags))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();


		ImGui::EndTable();
	}

	ImGui::Spacing();

	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
