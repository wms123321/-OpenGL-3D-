#pragma once

#include<vector>
#include<GL/glew.h>
#include"Renderer.h"

struct VertexBufferElement
{
	unsigned int type;       //����
	unsigned int count;      //����
	unsigned char normalized;//�Ƿ��һ��

	//�������ʹ�С
	static unsigned int GetSizeType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:return 4;
		case GL_UNSIGNED_INT:return 4;
		case GL_UNSIGNED_BYTE:return 1;
		}
		ASSERT(false);//δ���������Ͷ���ʧ�ܣ�����
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;//������Ϣ
	unsigned int m_Stride;//

public:
	VertexBufferLayout()
		:m_Stride(0)
	{

	}

	template<typename T>
	//δָ�����Ͷ���ʧ��
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT,count,GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeType(GL_FLOAT);//����*���ʹ�С
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