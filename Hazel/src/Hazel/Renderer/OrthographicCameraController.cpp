#include "hzpch.h"
#include "OrthographicCameraController.h"
#include <Hazel/Core/KeyCodes.h>
#include <Hazel/Core/Input.h>

namespace Hazel {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_CameraController(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_AspectRatio(aspectRatio), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		float x = Input::IsKeyPressed(HZ_KEY_A) ? -1.0f : Input::IsKeyPressed(HZ_KEY_D) ? 1.0f : 0.0f;
		float y = Input::IsKeyPressed(HZ_KEY_S) ? -1.0f : Input::IsKeyPressed(HZ_KEY_W) ? 1.0f : 0.0f;
		glm::vec2 cameraVelocity = { x, y };

		if (glm::length(cameraVelocity) > 0.00f)
		{
			glm::vec2 normalizedVelocity = glm::normalize(cameraVelocity) * m_CameraTranslationSpeed * m_CameraVelocityFactor * ts.GetSeconds();

			m_CameraPosition.x += normalizedVelocity.x;
			m_CameraPosition.y += normalizedVelocity.y;
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(HZ_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(HZ_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			m_CameraController.SetRotation(m_CameraRotation);
		}

		m_CameraController.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f; // 0.25f is the zoom speed

		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f); // 0.25f is the minimum zoom level
		m_ZoomLevel = std::min(m_ZoomLevel, 10.0f); // 10.0f is the maximum zoom level
		m_CameraController.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();

		m_CameraController.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}
}