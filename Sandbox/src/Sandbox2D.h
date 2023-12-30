#pragma once
#include "Hazel.h"
#include "ParticleSystem.h"

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

	glm::vec3 HSVtoRGB(double h, float s, float v);
private:
	// Temp
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Shader> m_FlatColorShader;

	Hazel::Ref<Hazel::Texture2D> m_Texture;
	Hazel::Ref<Hazel::Texture2D> m_SpriteSheet;

	Hazel::Ref<Hazel::QuadData3D> m_QuadData;
	Hazel::Ref<Hazel::QuadData3D> m_RotatingQuadData;
	Hazel::Ref<Hazel::QuadData3D> m_SquareData;
	Hazel::Ref<Hazel::QuadData2DBuilder> m_QuadBuilder2D;
	Hazel::Ref<Hazel::QuadData3DBuilder> m_QuadBuilder3D;
	glm::vec4 m_SquareColor = { 1.0f, 1.0f, 1.0f, 0.5f };
	glm::vec4 m_BackgroundTintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	Hazel::OrthographicCameraController m_CameraController;
	float m_QuadRotation = 0.0f;
	float m_QuadRotationSpeed = glm::radians(360.0f);

	float m_Timestep = 0.0f;

	// Particle System
	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;
};