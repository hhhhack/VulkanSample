#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class InputDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Camera
{
public:
	Camera();
	Camera(glm::vec3 cameraUp, glm::vec3 cameraPos, glm::vec3 cameraTarge);
	glm::mat4 GetView();
	void ProcessKeyBordInput(InputDirection direction, float fDeltaTime);
	void ProcessMouseInput(float fOffsetX, float fOffsetY);
private:
	void UpdateCamera();

public:
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraUp;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraRight;
	glm::vec3 m_cameraWorldUp;
	float m_fyaw;
	float m_fpitch;
	static float m_sfspeed;
	static float m_sfsensitivity;
	float m_fzoom;
};