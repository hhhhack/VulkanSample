#include "Texture.h"
#include "glad/glad.h"
#include "stb_image.h"

uint32_t Texture::m_suActiveTexture = GL_TEXTURE0;

void Texture::Load()
{
	m_pData = stbi_load(m_stexturePath.c_str(), &m_nWidth, &m_nHeight, &m_nChannel, 0);
	if (!m_pData)
	{
		std::cerr << "load file " << m_stexturePath << " fail" << std::endl;
	}
	glGenTextures(1, &m_uTexture);
	glActiveTexture(m_suActiveTexture);
	m_suActiveTexture++;
	glBindTexture(GL_TEXTURE_2D, m_uTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_nWidth, m_nHeight, 0, m_uTextureType, GL_UNSIGNED_BYTE, m_pData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(m_pData);
}