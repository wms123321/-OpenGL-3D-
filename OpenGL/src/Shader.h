#pragma once
#include<string>
#include<unordered_map>
#include<unordered_map>

#include"glm/glm.hpp"

//用于存储分离的源码
struct SharderProgramSource
{
	std::string vertex;
	std::string fragment;
	
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_LocationCache;



public:
	Shader(const std::string& filepath);
	~Shader();
	void Bind() const;
	void Unbind() const;

	//设置同一变量uniform
	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name,float v0, float v1, float v2, float v3 );
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniformVec3(const std::string& name, const glm::vec3& vec);


private:
	int GetUniformLocation(const std::string& name);

	SharderProgramSource ParseShader(const std::string& fliepath);

	unsigned int CompileShader(unsigned int type, const std::string& source);

	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

};