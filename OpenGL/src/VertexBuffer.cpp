#include"VertexBuffer.h"
#include"Renderer.h"


VertexBuffer::VertexBuffer(const void* data,unsigned int size)
{
    //���ɻ���������
    GLCall(glGenBuffers(1, &m_RendererID));
    //������ID�󶨵�������Ŀ�꣨GL_ARRAY_BUFFER��
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    //Ϊ��ǰ�󶨵Ļ����������ڴ棬����CPU�˵����ݸ��Ƶ�GPU�˵Ļ�����
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
