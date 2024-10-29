#include "glad/glad.h"

#include "OpenGl.h"
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <string>

const int cnWidth = 800;
const int cnHeight = 600;

static void framBufferSizeCallBack(GLFWwindow *pWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void ProcessInput(GLFWwindow* pWindow)
{
	auto keyState = glfwGetKey(pWindow, GLFW_KEY_ESCAPE);
	if (keyState == GLFW_PRESS)
		glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
}

void OpenGl::run()
{
	Init();
	Mainloop();
}

void OpenGl::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_OPENGL_CORE_PROFILE);

	m_pWindow = glfwCreateWindow(cnWidth, cnHeight, "OpenGl", nullptr, nullptr);
	if (!m_pWindow)
		throw std::runtime_error("Create glfw Window fail");

	glfwMakeContextCurrent(m_pWindow);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
		throw std::runtime_error("init glad fail");

	glViewport(0, 0, cnWidth, cnHeight);

	glfwSetFramebufferSizeCallback(m_pWindow, framBufferSizeCallBack);
}

void OpenGl::Mainloop()
{

	float vertices[] = {
		-0.5, 0, 0,
		0.5, 0, 0,
		0, 0.5, 0 
	};

	uint32_t VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	while (!glfwWindowShouldClose(m_pWindow))
	{
		glClearColor(0.123, 0.246, 0.369, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ProcessInput(m_pWindow);
		glfwPollEvents();
		glfwSwapBuffers(m_pWindow);
	}
}

void OpenGl::UnInit()
{
	glfwTerminate();
}

Shader::Shader(const std::string& shaderPath)
{
	m_shaderFilePath = shaderPath;
}

static uint32_t GetFileSize(FILE* pFile)
{
	uint32_t uFileSize = 0;
	if (!pFile)
		return uFileSize;
	fseek(pFile, 0, SEEK_END);
	uFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	return uFileSize;
}

void Shader::CreateShader()
{
	if (!std::filesystem::exists(m_shaderFilePath))
		throw std::runtime_error("shader file not exists");
	FILE* pShaderFile = fopen(m_shaderFilePath.c_str(), "r");
	//std::ifstream shaderFile(m_shaderFilePath);
	//if (!shaderFile.is_open())
	//	throw std::runtime_error("shader file is not open");
	if (!pShaderFile)
		throw std::runtime_error("open shader file fail");
	uint32_t uShaderFileSize = GetFileSize(pShaderFile);
	//std::string strShader;
	char* strShader = new char[uShaderFileSize];
	//strShader.resize(uShaderFileSize, 0);
	fread(strShader, 1, uShaderFileSize, pShaderFile);
	m_uShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_uShader, 1, &strShader, NULL);

	glCompileShader(m_uShader);
	delete[] strShader;

	fclose(pShaderFile);
}
