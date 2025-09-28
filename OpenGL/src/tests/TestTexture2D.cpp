#include"TestTexture2D.h"
#include"Renderer.h"
#include"imgui/imgui.h"
#include"VertexBufferLayout.h"
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"Texture.h"

namespace test {
	TestTexture2D::TestTexture2D()
        :m_TranslationA(1.0, -1.0, 0),m_TranslationB(2.0, -2.0, 0),
        m_Proj(glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0, 0)))
	{
        float positions[] = {
            -2.0f, 0.0f,0.0f,0.0f,
             0.0f, 0.0f,1.0f,0.0f,
             0.0f, 1.5f,1.0f,1.0f,
             -2.0f,1.5f,0.0f,1.0f,
        };

        unsigned int indices[] =
        {
            0,1,2,
            2,3,0
        };


        //���
        GLCall(glEnable(GL_BLEND));//���û�Ϲ���
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        ////�رջ�Ϲ���
        //GLCall(glDisable(GL_BLEND));

        m_Va = std::make_unique<VertexArray>();

        m_Vb = std::make_unique<VertexBuffer>(positions, sizeof(positions));

        VertexBufferLayout layout;
        layout.Push<float>(2);//ǰ2��float��λ��
        layout.Push<float>(2);//��2��float����������
        m_Va->AddBuffer(*m_Vb, layout);//��VBO�Ͳ��ֹ�����VAO

        m_Ib = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

        m_Va->Unbind();

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");//������ɫ������
        m_Shader->Bind();

        std::string path = "res/textures/eee.png";
        m_Texture = std::make_unique<Texture>(path);//��������ͼƬ
        m_Texture->Bind();

        m_Shader->SetUniform1i("u_Texture", 0);//ʹ��0������Ԫ������

        m_Shader->Unbind();
	}
	TestTexture2D::~TestTexture2D()
	{
	}
	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}
	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Shader->Bind();
        {
            //������ƽ��m_TranslationA
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_Va, *m_Ib, *m_Shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_Va, *m_Ib, *m_Shader);
        }

	}
	void TestTexture2D::OnImGuiRender()
	{
        //ͨ���ı�m_Translationƽ�ƾ��Σ���Χ[-2.0,2.0]
        ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, -2.0f, 2.0f);
        ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, -2.0f, 2.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}
}