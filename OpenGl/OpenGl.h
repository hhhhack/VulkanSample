#pragma once
#include "GLFW/glfw3.h"
#include <cstdio>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <string>
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
	Progarm(const std::string& strVertexShader, const std::string& strFragmentShader);
	~Progarm() {};
	void AddShader(const std::string& shaderPath, uint32_t uShaderType);
	void AddShader(Shader &shader);
	void CreateProgram();
	void UseProgaram();
	void SetBool(const std::string& strName, bool bValue);
	void SetInt(const std::string& strName, int32_t nValue);
	void SetFloat(const std::string& strName, float fValue);
	uint32_t GetProgram() { return m_uProgram; };
private:
	std::vector<Shader> m_shaders;
	uint32_t m_uProgram;
};

class Texture
{
public:
	Texture(const std::string& picturePath) :
		m_stexturePath(picturePath), 
		m_pData(nullptr),
		m_nWidth(0),
		m_nHeight(0),
		m_nChannel(0)
	{};
	~Texture() {};
	void Load();
private:
	std::string m_stexturePath;
	unsigned char* m_pData;
	int32_t m_nWidth;
	int32_t m_nHeight;
	int32_t m_nChannel;
};