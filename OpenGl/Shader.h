#pragma once
#include "glm/glm.hpp"
#include <string>
#include <vector>

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
	Progarm() :m_uProgram(0) {};
	Progarm(const std::string& strVertexShader, const std::string& strFragmentShader);
	~Progarm() {};
	void AddShader(const std::string& shaderPath, uint32_t uShaderType);
	void AddShader(Shader& shader);
	void CreateProgram();
	void UseProgaram();
	void SetBool(const std::string& strName, bool bValue);
	void SetInt(const std::string& strName, int32_t nValue);
	void SetFloat(const std::string& strName, float fValue);
	void SetVec3(const std::string& strNmae, glm::vec3&& Value);
	uint32_t GetProgram() { return m_uProgram; };
private:
	std::vector<Shader> m_shaders;
	uint32_t m_uProgram;
};
