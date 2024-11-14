#include "Shader.h"
//#include "glad/glad.h"

#include <cstdio>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <iostream>

Shader::Shader(const std::string& shaderPath, uint32_t uShaderType)
{
	m_shaderFilePath = shaderPath;
	m_uShaderType = uShaderType;
	m_uShader = 0;
}

Shader::~Shader()
{
	if (m_uShader)
		glDeleteShader(m_uShader);
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

template <class T>
static uint32_t GetFileSize(T& rFile)
{
	uint32_t uFileSize = 0;
	if (!rFile.is_open())
		return uFileSize;
	rFile.seekg(0, std::ios::end);
	uFileSize = rFile.tellg();
	rFile.seekg(0, std::ios::beg);
	return uFileSize;
}

void Shader::CreateShader()
{
	if (!std::filesystem::exists(m_shaderFilePath))
		throw std::runtime_error("shader file not exists");

	std::ifstream shaderFile(m_shaderFilePath);
	if (!shaderFile.is_open())
		throw std::runtime_error("shader file is not open");

	uint32_t uShaderFileSize = GetFileSize(shaderFile);

	char* strShader = new char[uShaderFileSize];
	memset(strShader, 0, uShaderFileSize);

	shaderFile.read(strShader, uShaderFileSize);
	m_uShader = glCreateShader(m_uShaderType);
	glShaderSource(m_uShader, 1, &strShader, NULL);
	glCompileShader(m_uShader);
	int success = false;
	glGetShaderiv(m_uShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char errorBuffer[512] = { 0 };
		glGetShaderInfoLog(m_uShader, 512, NULL, errorBuffer);
		std::cerr << "complied shadder error " << errorBuffer << std::endl;
	}

	delete[] strShader;
	shaderFile.close();
}



Progarm::Progarm(const std::string& strVertexShader, const std::string& strFragmentShader) :
	m_uProgram(0)
{
	m_shaders.emplace_back(strVertexShader, GL_VERTEX_SHADER);
	m_shaders.emplace_back(strFragmentShader, GL_FRAGMENT_SHADER);
	for (auto& shader : m_shaders)
		shader.CreateShader();
}

void Progarm::AddShader(const std::string& shaderPath, uint32_t uShaderType)
{
	auto& shader = m_shaders.emplace_back(shaderPath, uShaderType);
	shader.CreateShader();
}

void Progarm::AddShader(Shader& shader)
{
	m_shaders.emplace_back(shader);
}

void Progarm::CreateProgram()
{
	m_uProgram = glCreateProgram();
	int i = 0;
	for (auto shader : m_shaders)
	{
		i++;
		glAttachShader(m_uProgram, shader.GetShader());
		int nShaders = 0;
		glGetProgramiv(m_uProgram, GL_ATTACHED_SHADERS, &nShaders);
		if (nShaders != i)
			std::cerr << "program attach shader fail" << std::endl;
	}

	glLinkProgram(m_uProgram);
	int success = 0;
	glGetProgramiv(m_uProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char errorLog[512] = { 0 };
		glGetProgramInfoLog(m_uProgram, 512, NULL, errorLog);
		std::cerr << "Link program fail, error is " << errorLog << std::endl;
		throw std::runtime_error("Link program fail");
	}
}

void Progarm::UseProgaram()
{
	glUseProgram(m_uProgram);
	//glGetProgramiv(m_uProgram, )
}

void Progarm::SetBool(const std::string& strName, bool bValue)
{
	glUniform1i(glGetUniformLocation(m_uProgram, strName.c_str()), bValue);
}

void Progarm::SetInt(const std::string& strName, int32_t nValue)
{
	glUniform1i(glGetUniformLocation(m_uProgram, strName.c_str()), nValue);
}

void Progarm::SetFloat(const std::string& strName, float fValue)
{
	glUniform1i(glGetUniformLocation(m_uProgram, strName.c_str()), fValue);
}

void Progarm::SetVec3(const std::string& strName, float fValue1, float fValue2, float fValue3)
{
	glUniform3f(glGetUniformLocation(m_uProgram, strName.c_str()), fValue1, fValue2, fValue3);
}

