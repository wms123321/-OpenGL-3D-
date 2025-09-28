#include"VertexArray.h"
#include"Renderer.h"
#include"VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i=0;i<elements.size();i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		//glVertexAttribPointer(索引, 个数, 类型, 是否归一化, 步长, 偏移量)
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count*VertexBufferElement::GetSizeType(element.type);//偏移量为当前已绑定的数据的个数*数据的大小
	}

	
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}
void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
