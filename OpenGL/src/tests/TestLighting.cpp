#include "TestLighting.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
    TestLighting::TestLighting()
        : m_LightPos(2.0f, 2.0f, 2.0f),//��Դλ��
        m_LightColor(1.0f, 1.0f, 1.0f),//��������ɫ
        m_ObjectColor(1.0f, 1.0f, 1.0f),//������ɫ
        m_AmbientStrength(0.1f),//������ǿ��
        m_SpecularStrength(0.5f),//�����ǿ��
        m_Shininess(32),//�߹�ϵ��
        m_CameraPos(0.0f, 0.0f, 3.0f),//���λ��
        m_ObjectRotation(0.0f, 0.0f, 0.0f),//�����ʼ��ת
        m_AutoRotateO(false),//�����Զ���ת
        m_AutoMoveL(false)//��Դ�Զ��ƶ�
    {
        //�����嶥�����ݣ�λ��3+����3��
        float vertices[] = {
            //��
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            //��
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            //��
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,

            //��
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,

             //��
             -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
              0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
              0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,

              //��
              -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
              -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
               0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
               0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f
        };

        //��������������
        unsigned int indices[] = {
            //��
            0, 1, 2,  2, 3, 0,
            //��
            4, 6, 5,  6, 4, 7,
            //��
            8, 9, 10,  10, 11, 8,
            //��
            12, 14, 13,  14, 12, 15,
            //��
            16, 17, 18,  18, 19, 16,
            //��
            20, 22, 21,  22, 20, 23
        };

        //������Ȳ���
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(GL_LESS));

        //���û��
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_Va = std::make_unique<VertexArray>();

        m_Vb = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

        VertexBufferLayout layout;
        layout.Push<float>(3); //λ��
        layout.Push<float>(3); //����
        m_Va->AddBuffer(*m_Vb, layout);

        m_Ib = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

        m_Va->Unbind();

        //������ɫ������
        m_Shader = std::make_unique<Shader>("res/shaders/Lighting.shader");
        m_Shader->Bind();

        //��ʼuniformֵ
        m_Shader->SetUniformVec3("u_LightColor", m_LightColor);
        m_Shader->SetUniformVec3("u_ObjectColor", m_ObjectColor);
        m_Shader->SetUniform1f("u_AmbientStrength", m_AmbientStrength);
        m_Shader->SetUniform1f("u_SpecularStrength", m_SpecularStrength);
        m_Shader->SetUniform1i("u_Shininess", m_Shininess);

        m_Shader->Unbind();
    }

    TestLighting::~TestLighting()
    {
        //������Ȳ���
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void TestLighting::OnUpdate(float deltaTime)
    {
        //�Զ���ת����
        if (m_AutoRotateO)
        {
            m_ObjectRotation.y += deltaTime * 30.0f;
            if (m_ObjectRotation.y > 360.0f)
                m_ObjectRotation.y -= 360.0f;
        }

        //�ƶ���Դ
        if (m_AutoMoveL)
        {
            static float lightTime = 0.0f;
            lightTime += deltaTime;
            m_LightPos.x = 2.0f * cos(lightTime);
            m_LightPos.z = 2.0f * sin(lightTime);
        }
    }

    void TestLighting::OnRender()
    {
        //�����ɫ����Ȼ���
        GLCall(glClearColor(0.05f, 0.05f, 0.05f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;

        //�任����
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(m_ObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_ObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_ObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 view = glm::lookAt(
            m_CameraPos,//���λ��
            glm::vec3(0.0f, 0.0f, 0.0f),//����ԭ��
            glm::vec3(0.0f, 1.0f, 0.0f)//�Ϸ���
        );

        //͸��ͶӰ����
        float aspectRatio = 1280.0f / 960.0f;  //���ݴ��ڴ�С����
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),  //FOV
            aspectRatio,//��߱�
            0.1f,//��ƽ��
            100.0f//Զƽ��
        );

        //����ɫ��������uniform����
        m_Shader->Bind();

        //���ñ任����
        m_Shader->SetUniformMat4f("u_Model", model);
        m_Shader->SetUniformMat4f("u_View", view);
        m_Shader->SetUniformMat4f("u_Projection", projection);

        //���ù��ղ���
        m_Shader->SetUniformVec3("u_LightPos", m_LightPos);
        m_Shader->SetUniformVec3("u_LightColor", m_LightColor);
        m_Shader->SetUniformVec3("u_ObjectColor", m_ObjectColor);
        m_Shader->SetUniformVec3("u_ViewPos", m_CameraPos);
        m_Shader->SetUniform1f("u_AmbientStrength", m_AmbientStrength);
        m_Shader->SetUniform1f("u_SpecularStrength", m_SpecularStrength);
        m_Shader->SetUniform1i("u_Shininess", m_Shininess);

        renderer.Draw(*m_Va, *m_Ib, *m_Shader);

    }

    void TestLighting::OnImGuiRender()
    {
        ImGui::Text("Lighting Controls");
        ImGui::Separator();

        //��Դ����
        if (ImGui::CollapsingHeader("Light Properties", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3("Light Position", &m_LightPos.x, 0.1f, -5.0f, 5.0f);
            ImGui::ColorEdit3("Light Color", &m_LightColor.x);
            ImGui::Checkbox("Auto Move", &m_AutoMoveL);
        }

        //���ʿ���
        if (ImGui::CollapsingHeader("Material Properties", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::ColorEdit3("Object Color", &m_ObjectColor.x);
            ImGui::SliderFloat("Ambient Strength", &m_AmbientStrength, 0.0f, 1.0f);
            ImGui::SliderFloat("Specular Strength", &m_SpecularStrength, 0.0f, 1.0f);
            ImGui::SliderInt("Shininess", &m_Shininess, 2, 256);
        }

        //�������
        if (ImGui::CollapsingHeader("Camera Controls"))
        {
            ImGui::DragFloat3("Camera Position", &m_CameraPos.x, 0.1f, -10.0f, 10.0f);
        }

        //����任����
        if (ImGui::CollapsingHeader("Object Transform"))
        {
            ImGui::DragFloat3("Rotation", &m_ObjectRotation.x, 1.0f, 0.0f, 360.0f);
            ImGui::Checkbox("Auto Rotate", &m_AutoRotateO);
        }

        //Ԥ�����
        if (ImGui::CollapsingHeader("Presets"))
        {
            if (ImGui::Button("Golden Material"))//�ƽ����
            {
                m_ObjectColor = glm::vec3(1.0f, 0.843f, 0.0f);
                m_AmbientStrength = 0.24f;
                m_SpecularStrength = 0.628f;
                m_Shininess = 51;
            }
            ImGui::SameLine();
            if (ImGui::Button("Steel Metal"))//�ֲ���
            {
                m_ObjectColor = glm::vec3(0.8f, 0.8f, 0.85f);
                m_AmbientStrength = 0.15f;
                m_SpecularStrength = 0.9f;
                m_Shininess = 150;
            }
            ImGui::SameLine();
            if (ImGui::Button("Copper Metal"))//ͭ����
            {
                m_ObjectColor = glm::vec3(0.8f, 0.5f, 0.3f);
                m_AmbientStrength = 0.12f;
                m_SpecularStrength = 0.85f;
                m_Shininess = 120;
            }
            ImGui::SameLine();
            if (ImGui::Button("Wooden Metal"))//ľ����
            {
                m_ObjectColor = glm::vec3(0.6f, 0.4f, 0.2f);
                m_AmbientStrength = 0.35f; 
                m_SpecularStrength = 0.08f;
                m_SpecularStrength = 0.08f;
                m_Shininess = 15;
            }
            

        }

        //֡��
        ImGui::Separator();
        ImGui::Text("Performance");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}