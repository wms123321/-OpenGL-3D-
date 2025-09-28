#pragma once

#include<vector>
#include<GL/glew.h>
#include"Renderer.h"

struct VertexBufferElement
{
	unsigned int type;       //类型
	unsigned int count;      //个数
	unsigned char normalized;//是否归一化

	//返回类型大小
	static unsigned int GetSizeType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:return 4;
		case GL_UNSIGNED_INT:return 4;
		case GL_UNSIGNED_BYTE:return 1;
		}
		ASSERT(false);//未包含的类型断言失败，报错
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;//布局信息
	unsigned int m_Stride;//

public:
	VertexBufferLayout()
		:m_Stride(0)
	{

	}

	template<typename T>
	//未指定类型断言失败
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT,count,GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeType(GL_FLOAT);//个数*类型大小
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT,count,GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE,count,GL_TRUE });
		m_Stride += count*VertexBufferElement::GetSizeType(GL_UNSIGNED_BYTE);
	}

	inline std::vector<VertexBufferElement> GetElements() const { return m_Elements; }

	inline unsigned int GetStride() const { return m_Stride; }

};