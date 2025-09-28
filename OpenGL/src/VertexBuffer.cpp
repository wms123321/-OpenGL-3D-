#include"VertexBuffer.h"
#include"Renderer.h"


VertexBuffer::VertexBuffer(const void* data,unsigned int size)
{
    //生成缓冲区对象
    GLCall(glGenBuffers(1, &m_RendererID));
    //缓冲区ID绑定到缓冲区目标（GL_ARRAY_BUFFER）
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    //为当前绑定的缓冲区分配内存，并将CPU端的数据复制到GPU端的缓冲区
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
