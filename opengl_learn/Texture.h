#pragma once

#include "Renderer.h"

#include <string>

class TextureM
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer; // CPU �˴洢�� Texture
	int m_Width, m_Height, m_BPP; // Bit Per Pixel of the actual texture
public:
	TextureM(const std::string& path);
	~TextureM();

	void Bind(unsigned int slot = 0) const; 
	void Unbind() const;

	inline int GetWidth() const { return m_Width; };
	inline int GetHeight() const { return m_Height; };
};