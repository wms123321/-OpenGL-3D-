#include "TestLighting.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
    TestLighting::TestLighting()
        : m_LightPos(2.0f, 2.0f, 2.0f),//光源位置
        m_LightColor(1.0f, 1.0f, 1.0f),//环境光颜色
        m_ObjectColor(1.0f, 1.0f, 1.0f),//物体颜色
        m_AmbientStrength(0.1f),//环境光强度
        m_SpecularStrength(0.5f),//镜面光强度
        m_Shininess(32),//高光系数
        m_CameraPos(0.0f, 0.0f, 3.0f),//相机位置
        m_ObjectRotation(0.0f, 0.0f, 0.0f),//物体初始旋转
        m_AutoRotateO(false),//物体自动旋转
        m_AutoMoveL(false)//光源自动移动
    {
        //立方体顶点数据（位置3+法线3）
        float vertices[] = {
            //正
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            //背
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            //左
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,

            //右
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,

             //上
             -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
              0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
              0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,

              //下
              -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
              -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
               0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
               0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f
        };

        //立方体索引数据
        unsigned int indices[] = {
            //正
            0, 1, 2,  2, 3, 0,
            //背
            4, 6, 5,  6, 4, 7,
            //左
            8, 9, 10,  10, 11, 8,
            //右
            12, 14, 13,  14, 12, 15,
            //上
            16, 17, 18,  18, 19, 16,
            //下
            20, 22, 21,  22, 20, 23
        };

        //启用深度测试
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(GL_LESS));

        //启用混合
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_Va = std::make_unique<VertexArray>();

        m_Vb = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

        VertexBufferLayout layout;
        layout.Push<float>(3); //位置
        layout.Push<float>(3); //法线
        m_Va->AddBuffer(*m_Vb, layout);

        m_Ib = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

        m_Va->Unbind();

        //创建着色器程序
        m_Shader = std::make_unique<Shader>("res/shaders/Lighting.shader");
        m_Shader->Bind();

        //初始uniform值
        m_Shader->SetUniformVec3("u_LightColor", m_LightColor);
        m_Shader->SetUniformVec3("u_ObjectColor", m_ObjectColor);
        m_Shader->SetUniform1f("u_AmbientStrength", m_AmbientStrength);
        m_Shader->SetUniform1f("u_SpecularStrength", m_SpecularStrength);
        m_Shader->SetUniform1i("u_Shininess", m_Shininess);

        m_Shader->Unbind();
    }

    TestLighting::~TestLighting()
    {
        //禁用深度测试
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void TestLighting::OnUpdate(float deltaTime)
    {
        //自动旋转物体
        if (m_AutoRotateO)
        {
            m_ObjectRotation.y += deltaTime * 30.0f;
            if (m_ObjectRotation.y > 360.0f)
                m_ObjectRotation.y -= 360.0f;
        }

        //移动光源
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
        //清除颜色和深度缓冲
        GLCall(glClearColor(0.05f, 0.05f, 0.05f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;

        //变换矩阵
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(m_ObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_ObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_ObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 view = glm::lookAt(
            m_CameraPos,//相机位置
            glm::vec3(0.0f, 0.0f, 0.0f),//看向原点
            glm::vec3(0.0f, 1.0f, 0.0f)//上方向
        );

        //透视投影矩阵
        float aspectRatio = 1280.0f / 960.0f;  //根据窗口大小调整
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),  //FOV
            aspectRatio,//宽高比
            0.1f,//近平面
            100.0f//远平面
        );

        //绑定着色器并设置uniform变量
        m_Shader->Bind();

        //设置变换矩阵
        m_Shader->SetUniformMat4f("u_Model", model);
        m_Shader->SetUniformMat4f("u_View", view);
        m_Shader->SetUniformMat4f("u_Projection", projection);

        //设置光照参数
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

        //光源控制
        if (ImGui::CollapsingHeader("Light Properties", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3("Light Position", &m_LightPos.x, 0.1f, -5.0f, 5.0f);
            ImGui::ColorEdit3("Light Color", &m_LightColor.x);
            ImGui::Checkbox("Auto Move", &m_AutoMoveL);
        }

        //材质控制
        if (ImGui::CollapsingHeader("Material Properties", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::ColorEdit3("Object Color", &m_ObjectColor.x);
            ImGui::SliderFloat("Ambient Strength", &m_AmbientStrength, 0.0f, 1.0f);
            ImGui::SliderFloat("Specular Strength", &m_SpecularStrength, 0.0f, 1.0f);
            ImGui::SliderInt("Shininess", &m_Shininess, 2, 256);
        }

        //相机控制
        if (ImGui::CollapsingHeader("Camera Controls"))
        {
            ImGui::DragFloat3("Camera Position", &m_CameraPos.x, 0.1f, -10.0f, 10.0f);
        }

        //物体变换控制
        if (ImGui::CollapsingHeader("Object Transform"))
        {
            ImGui::DragFloat3("Rotation", &m_ObjectRotation.x, 1.0f, 0.0f, 360.0f);
            ImGui::Checkbox("Auto Rotate", &m_AutoRotateO);
        }

        //预设材质
        if (ImGui::CollapsingHeader("Presets"))
        {
            if (ImGui::Button("Golden Material"))//黄金材质
            {
                m_ObjectColor = glm::vec3(1.0f, 0.843f, 0.0f);
                m_AmbientStrength = 0.24f;
                m_SpecularStrength = 0.628f;
                m_Shininess = 51;
            }
            ImGui::SameLine();
            if (ImGui::Button("Steel Metal"))//钢材质
            {
                m_ObjectColor = glm::vec3(0.8f, 0.8f, 0.85f);
                m_AmbientStrength = 0.15f;
                m_SpecularStrength = 0.9f;
                m_Shininess = 150;
            }
            ImGui::SameLine();
            if (ImGui::Button("Copper Metal"))//铜材质
            {
                m_ObjectColor = glm::vec3(0.8f, 0.5f, 0.3f);
                m_AmbientStrength = 0.12f;
                m_SpecularStrength = 0.85f;
                m_Shininess = 120;
            }
            ImGui::SameLine();
            if (ImGui::Button("Wooden Metal"))//木材质
            {
                m_ObjectColor = glm::vec3(0.6f, 0.4f, 0.2f);
                m_AmbientStrength = 0.35f; 
                m_SpecularStrength = 0.08f;
                m_SpecularStrength = 0.08f;
                m_Shininess = 15;
            }
            

        }

        //帧数
        ImGui::Separator();
        ImGui::Text("Performance");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}