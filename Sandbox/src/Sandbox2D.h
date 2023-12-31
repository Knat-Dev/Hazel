#pragma once
#include "Hazel.h"

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Hazel::Timestep ts) override;
	void HandleInput(Hazel::Timestep ts);
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;
private:
	// Temp
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Shader> m_FlatColorShader;
	Hazel::Ref<Hazel::Texture2D> m_Texture;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 0.75f };
	Hazel::OrthographicCameraController m_CameraController;
	float m_QuadRotation = 0.0f;
};