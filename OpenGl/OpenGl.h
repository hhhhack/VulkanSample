#pragma once
#include "GLFW/glfw3.h"
#include <vector>

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

class Shader
{
public:
	Shader(const std::string& shaderPath, uint32_t uShaderType);
	~Shader();
	void CreateShader();
	uint32_t GetShader() { return m_uShader; };
private:
	uint32_t m_uShaderType;
	std::string m_shaderFilePath;
	uint32_t m_uShader;
};

class Progarm
{
public:
	Progarm():m_uProgram(0){};
	~Progarm() {};
	void AddShader(const std::string& shaderPath, uint32_t uShaderType);
	void CreateProgram();
	void UseProgaram();
private:
	std::vector<Shader> m_shaders;
	uint32_t m_uProgram;
};