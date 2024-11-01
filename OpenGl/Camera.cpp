#include "Camera.h"

Camera::Camera()
{
	//m_model = glm::mat4(1.0f);
	m_view = glm::mat4(1.0f);
	m_prejection = glm::perspective(glm::radians(45.0f), 8.0f / 6.0f, 0.01f, 100.0f);
	m_trans = m_prejection * m_view;
}

Camera::Camera(glm::vec3 cameraUp, glm::vec3 cameraPos, glm::vec3 cameraTarget)
{
	m_cameraDirection = glm::normalize(cameraPos - cameraTarget);
	m_cameraUp = glm::normalize(cameraUp);
	m_cameraRight = glm::normalize(glm::cross(m_cameraDirection, m_cameraUp));
	m_view = glm::lookAt(
		cameraPos,
		cameraTarget,
		cameraUp
	);

	m_prejection = glm::perspective(glm::radians(45.0f), 8.0f / 6.0f, 0.01f, 100.0f);
	m_trans = m_prejection * m_view;
}
