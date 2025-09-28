#pragma once

#include"Test.h"
#include"VertexArray.h"
#include"Shader.h"
#include<memory>
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"Texture.h"
namespace test {
	class TestTexture2D :public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime)override;
		void OnRender()override;
		void OnImGuiRender()override;

	private:
		std::unique_ptr<VertexArray> m_Va;
		std::unique_ptr<VertexBuffer> m_Vb;
		std::unique_ptr<IndexBuffer> m_Ib;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		
		glm::vec3 m_TranslationA;//ƽ��A
		glm::vec3 m_TranslationB;//ƽ��B
		glm::mat4 m_Proj;//ͶӰ��������ͶӰ��
		glm::mat4 m_View;//��ͼ����
	};
}