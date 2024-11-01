#pragma once
#include "GLFW/glfw3.h"
#include <cstdio>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
