#include"Renderer.h"
#include<iostream>

//��������еĴ���
void GLClearError()
{
    //ѭ����ȡ���д���ֱ��GL_NO_ERROR���޴���
    while (glGetError() != GL_NO_ERROR)
    {

    }
}

bool GLLogCall(const char* function, const char* file, int line)
{
    //��ȡ����
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error ] " << "(" << error << ") :" << function << "  " << file << "  " << line << std::endl;
        return false;
    }
    return true;
}


//����ͼ��
void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();//������ɫ������
    va.Bind();//�󶨶�������
    ib.Bind();//����������
    //ִ�л������ʹ�����������е����ݻ���������
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

}

void Renderer::Clear() const
{
    //�����ɫ������
    glClear(GL_COLOR_BUFFER_BIT);
}
