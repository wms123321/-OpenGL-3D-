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

//1��int
void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

//4��float
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
//mat4����
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}


//1��float
void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}
//3��float
void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}
//��ά����
void Shader::SetUniformVec3(const std::string& name, const glm::vec3& vec)
{
    GLCall(glUniform3fv(GetUniformLocation(name), 1, &vec[0]));
}



//��ѯUniform��������ɫ�������е�����
int Shader::GetUniformLocation(const std::string& name)
{
    //��������ֱ�ӷ���
    if (m_LocationCache.find(name) != m_LocationCache.end())
    {
        return m_LocationCache[name];
    }
    //δ���У���ѯ����
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    //����Ϊ-1����ѯʧ��
    if (location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' dosen't exist !" << std::endl;
    }
    //�洢uniform��������
    m_LocationCache[name] = location;

    return location;
}


//����������еĶ�����ɫ����Ƭ����ɫ������
SharderProgramSource Shader::ParseShader(const std::string& fliepath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream stream(fliepath);
    std::string line;
    std::stringstream ss[2];// 0:������ɫ��Դ�룬1:Ƭ����ɫ��Դ��
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        //��ƥ��ɹ�ʱ
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

//������ɫ��
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);//��Դ��
    glCompileShader(id);//����

    //������
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);//��ȡ����״̬
    if (result == GL_FALSE)//����ʧ��ʱ
    {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);//��ȡ������־����
        char* message = (char*)alloca(len * sizeof(char));//��ջ�Ϸ��������־������ڴ�
        glGetShaderInfoLog(id, len, &len, message);//��ȡ������־
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);//�������ʧ�ܵ���ɫ��
        return 0;
    }


    return id;
}

//��ɫ�����򴴽�������
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();//�����������
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);//���붥����ɫ��
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);//����Ƭ����ɫ��

    glAttachShader(program, vs);//���Ӷ�����ɫ��
    glAttachShader(program, fs);//����Ƭ����ɫ��
    glLinkProgram(program);//���ӳ���
    glValidateProgram(program);//��֤����
    //������ɫ��
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


void Shader::Bind()const
{
    //������ɫ������
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind()const
{
    //ͣ����ɫ������
    GLCall(glUseProgram(0));
}