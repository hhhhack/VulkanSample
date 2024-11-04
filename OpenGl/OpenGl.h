#pragma once
#include "GLFW/glfw3.h"

class OpenGl
{
public:
	OpenGl():
		m_pWindow(nullptr)
	{};
	~OpenGl() {};

	void run();
private:
	void Init();
	void Mainloop();
	void UnInit();
private:
	GLFWwindow* m_pWindow;
};
