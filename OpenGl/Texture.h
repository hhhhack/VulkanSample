#pragma once

#include "glad/glad.h"
#include <string>

class Texture
{
public:
	Texture(const std::string& picturePath) :
		m_stexturePath(picturePath),
		m_pData(nullptr),
		m_nWidth(0),
		m_nHeight(0),
		m_nChannel(0),
		m_uTexture(0),
		m_uTextureType(GL_RGB)
	{
	};

	Texture(const std::string& picturePath, uint32_t uTextureType) :
		m_stexturePath(picturePath),
		m_pData(nullptr),
		m_nWidth(0),
		m_nHeight(0),
		m_nChannel(0),
		m_uTexture(0),
		m_uTextureType(uTextureType)
	{
	};
	~Texture() {};
	void Load();
	uint32_t GetTexture() { return m_uTexture; };
private:
	static uint32_t m_suActiveTexture;
	uint32_t m_uTextureType;
	std::string m_stexturePath;
	unsigned char* m_pData;
	uint32_t m_uTexture;
	int32_t m_nWidth;
	int32_t m_nHeight;
	int32_t m_nChannel;
};