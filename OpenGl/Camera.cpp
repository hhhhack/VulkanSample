#include "Camera.h"

float Camera::m_sfspeed = 2.5f;
float Camera::m_sfsensitivity = 0.1f;

Camera::Camera():
	m_fyaw(-90.0f),
	m_fpitch(0.0f),
	m_fzoom(45.0f)
{
	m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	m_cameraWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_cameraTarget = m_cameraPos + m_cameraFront;
	UpdateCamera();
}

Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 cameraUp)
{
	m_cameraPos = cameraPos;
	m_cameraTarget = cameraTarget;
	m_cameraUp = cameraUp;
}

glm::mat4 Camera::GetView()
{
	return glm::lookAt(
		m_cameraPos,
		m_cameraPos + m_cameraFront,
		m_cameraUp
	);
}

void Camera::ProcessKeyBordInput(InputDirection direction, float fDeltaTime)
{
	float fMoveLength = fDeltaTime * m_sfspeed;
	switch (direction)
	{
	case InputDirection::UP:
		m_cameraPos += m_cameraFront * fMoveLength;
		break;
	case InputDirection::DOWN:
		m_cameraPos -= m_cameraFront * fMoveLength;
		break;
	case InputDirection::LEFT:
		m_cameraPos -= m_cameraRight * fMoveLength;
		break;
	case InputDirection::RIGHT:
		m_cameraPos += m_cameraRight * fMoveLength;
		break;
	default:
		break;
	}
}

void Camera::ProcessMouseInput(float fOffsetX, float fOffsetY)
{
	m_fyaw += fOffsetX * m_sfsensitivity;
	m_fpitch += fOffsetY * m_sfsensitivity;

	m_fpitch > 89.9 ? 89.9 : m_fpitch;
	m_fpitch < -89.9 ? -89.9 : m_fpitch;

	UpdateCamera();
}

void Camera::UpdateCamera()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_fyaw)) * cos(glm::radians(m_fpitch));
	front.y = sin(glm::radians(m_fpitch));
	front.z = sin(glm::radians(m_fyaw)) * cos(glm::radians(m_fpitch));

	m_cameraFront = glm::normalize(front);
	m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_cameraWorldUp));
	m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));
}
