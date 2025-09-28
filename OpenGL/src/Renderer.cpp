#include"Renderer.h"
#include<iostream>

//清理队列中的错误
void GLClearError()
{
    //循环读取所有错误，直到GL_NO_ERROR（无错误）
    while (glGetError() != GL_NO_ERROR)
    {

    }
}

bool GLLogCall(const char* function, const char* file, int line)
{
    //获取错误
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error ] " << "(" << error << ") :" << function << "  " << file << "  " << line << std::endl;
        return false;
    }
    return true;
}


//绘制图形
void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();//激活着色器程序
    va.Bind();//绑定顶点数组
    ib.Bind();//绑定索引缓冲
    //执行绘制命令：使用索引缓冲中的数据绘制三角形
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

}

void Renderer::Clear() const
{
    //清空颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
}
