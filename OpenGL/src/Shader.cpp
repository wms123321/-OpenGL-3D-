#include "Shader.h"
#include<fstream>
#include<string>
#include<sstream>
#include<iostream>

#include"Renderer.h"

Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath),m_RendererID(0)
{
    SharderProgramSource source = ParseShader(filepath);
    std::cout << source.vertex << std::endl;
    std::cout << source.fragment << std::endl;

    m_RendererID = CreateShader(source.vertex, source.fragment);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

//1个int
void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

//4个float
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
//mat4矩阵
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}


//1个float
void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}
//3个float
void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}
//三维向量
void Shader::SetUniformVec3(const std::string& name, const glm::vec3& vec)
{
    GLCall(glUniform3fv(GetUniformLocation(name), 1, &vec[0]));
}



//查询Uniform变量在着色器程序中的索引
int Shader::GetUniformLocation(const std::string& name)
{
    //缓存命中直接返回
    if (m_LocationCache.find(name) != m_LocationCache.end())
    {
        return m_LocationCache[name];
    }
    //未命中，查询索引
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    //索引为-1，查询失败
    if (location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' dosen't exist !" << std::endl;
    }
    //存储uniform及其索引
    m_LocationCache[name] = location;

    return location;
}


//分离出代码中的顶点着色器和片段着色器代码
SharderProgramSource Shader::ParseShader(const std::string& fliepath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream stream(fliepath);
    std::string line;
    std::stringstream ss[2];// 0:顶点着色器源码，1:片段着色器源码
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        //当匹配成功时
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(),ss[1].str() };
}

//编译着色器
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);//绑定源码
    glCompileShader(id);//编译

    //错误处理
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);//获取编译状态
    if (result == GL_FALSE)//编译失败时
    {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);//获取错误日志长度
        char* message = (char*)alloca(len * sizeof(char));//在栈上分配错误日志所需的内存
        glGetShaderInfoLog(id, len, &len, message);//获取错误日志
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);//清理编译失败的着色器
        return 0;
    }


    return id;
}

//着色器程序创建与链接
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();//创建程序对象
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);//编译顶点着色器
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);//编译片段着色器

    glAttachShader(program, vs);//附加顶点着色器
    glAttachShader(program, fs);//附加片段着色器
    glLinkProgram(program);//链接程序
    glValidateProgram(program);//验证程序
    //清理着色器
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


void Shader::Bind()const
{
    //激活着色器程序
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind()const
{
    //停用着色器程序
    GLCall(glUseProgram(0));
}