#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(std::string& path)
	:m_RendererID(0),m_FilePath(path),m_LocalBuffer(nullptr),m_Width(0),m_Height(0),m_BPP(0)	
{
	stbi_set_flip_vertically_on_load(1);//垂直翻转
	//加载图片获得像素数据
	m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	//创建纹理对象，设置参数
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));//2D纹理

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));//缩小过滤
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));//放大过滤
	//纹理环绕参数
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	//glTexImage2D(2D纹理, 0, GPU内部存储格式, m_Width, m_Height, 边框宽度（核心模式不支持非0边框）, CPU端像素数据格式, 像素数据类型, CPU端像素数据缓冲区)
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
	{
		//释放CPU端像素缓冲区
		stbi_image_free(m_LocalBuffer);
	}

}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));//激活指定的纹理单元
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}