#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 cameraUp, glm::vec3 cameraPos, glm::vec3 cameraTarge);
	//void SetModel(glm::mat4 model) { m_model = model; };
	void SetView(glm::mat4 view) { m_view = view; };
	void SetPrejection(glm::mat4 prejection) { m_prejection = prejection; };
	glm::mat4 GetTrans() { return m_prejection * m_view; };
private:
	glm::vec3 m_cameraDirection;
	glm::vec3 m_cameraRight;
	glm::vec3 m_cameraUp;
	glm::mat4 m_view;
	glm::mat4 m_prejection;
	glm::mat4 m_trans;
};