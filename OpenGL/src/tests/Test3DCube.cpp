#include "Test3DCube.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture.h"

namespace test {
    Test3DCube::Test3DCube()
        : m_Translation(0.0f, 0.0f, 0.0f),
        m_Rotation(0.0f, 0.0f, 0.0f),
        m_Scale(1.0f, 1.0f, 1.0f),
        m_CameraDistance(5.0f),
        m_AutoRotate(false)
    {
        //立方体顶点数据(位置3+纹理坐标2）
        float vertices[] = {
            //正
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            
            //背
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            
            //左
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

            //右
             0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
             0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
             0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

            //上
            -0.5f,  0.5f, -0.5f,0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,0.0f, 1.0f,

            //下
            -0.5f, -0.5f, -0.5f,0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,0.0f, 1.0f

        };

        unsigned int indices[] = {
            //正
            0, 1, 2,  2, 3, 0,
            //背
            4, 5, 6,  6, 7, 4,
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

        ////启用混合
        //GLCall(glEnable(GL_BLEND));
        //GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        //关闭混合功能
        GLCall(glDisable(GL_BLEND));

        m_Va = std::make_unique<VertexArray>();
        m_Vb = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

        VertexBufferLayout layout;
        layout.Push<float>(3);//位置
        layout.Push<float>(2);//纹理坐标
        m_Va->AddBuffer(*m_Vb, layout);

        m_Ib = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

        //创建着色器
        m_Shader = std::make_unique<Shader>("res/shaders/3D.shader");
        m_Shader->Bind();

        //加载纹理
        std::string path = "res/textures/ccc.png";
        m_Texture = std::make_unique<Texture>(path);
        m_Texture->Bind();
        m_Shader->SetUniform1i("u_Texture", 0);

        //投影矩阵（透视投影）
        m_Proj = glm::perspective(glm::radians(45.0f), 1280.0f / 960.0f, 0.1f, 100.0f);

        m_Shader->Unbind();
    }

    Test3DCube::~Test3DCube()
    {
        //禁用深度测试
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void Test3DCube::OnUpdate(float deltaTime)
    {
        //启用自动旋转
        if (m_AutoRotate)
        {
            //按时间旋转
            m_Rotation.y += deltaTime * 50.0f;
            if (m_Rotation.y > 360.0f)
                m_Rotation.y -= 360.0f;
        }
    }

    void Test3DCube::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        //清除颜色缓冲和深度缓冲
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;

        //更新视图矩阵（相机位置）
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_CameraDistance));

        m_Shader->Bind();
        {
            
            glm::mat4 model = glm::mat4(1.0f);
            //平移
            model = glm::translate(model, m_Translation);
            //旋转
            model = glm::rotate(model, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));//x轴
            model = glm::rotate(model, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));//y轴
            model = glm::rotate(model, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));//z轴
            //缩放
            model = glm::scale(model, m_Scale);

            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_Va, *m_Ib, *m_Shader);
        }
    }

    void Test3DCube::OnImGuiRender()
    {
        ImGui::Text("3D Cube Controls");
        ImGui::SliderFloat3("Translation", &m_Translation.x, -3.0f, 3.0f);
        ImGui::SliderFloat3("Rotation", &m_Rotation.x, 0.0f, 360.0f);
        ImGui::SliderFloat3("Scale", &m_Scale.x, 0.1f, 3.0f);
        ImGui::SliderFloat("Camera Distance", &m_CameraDistance, 1.0f, 10.0f);
        ImGui::Checkbox("Auto Rotate", &m_AutoRotate);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}