#pragma once


#include "Renderer.h"


class Texture
{
private:

	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;//存储加载图片后获得的数据
	int m_Width, m_Height, m_BPP;//像素宽度、高度，像素的字节数


public:
	Texture(std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0)const;
	void Unbind()const;

	inline int GetWidth()const{ return m_Width; }
	inline int GetHeight()const { return m_Height; }


};
