#pragma once
#include <Hazel/Core/Timestep.h>
#include <Hazel/Events/Event.h>
#include <Hazel/Events/MouseEvent.h>
#include <Hazel/Events/ApplicationEvent.h>
#include "Hazel/Renderer/OrthographicCamera.h"

namespace Hazel {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_CameraController; }
		const OrthographicCamera& GetCamera() const { return m_CameraController; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_CameraController;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; // In degrees, in the anti-clockwise direction
		float m_CameraTranslationSpeed = 1.0f, m_CameraVelocityFactor = 2.5f, m_CameraRotationSpeed = 180.0f;
	};
}