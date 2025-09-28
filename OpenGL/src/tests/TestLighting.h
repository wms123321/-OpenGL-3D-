#pragma once

#include "Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <memory>
#include "glm/glm.hpp"

namespace test {
    class TestLighting : public Test
    {
    public:
        TestLighting();
        ~TestLighting();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        std::unique_ptr<VertexArray> m_Va;
        std::unique_ptr<VertexBuffer> m_Vb;
        std::unique_ptr<IndexBuffer> m_Ib;
        std::unique_ptr<Shader> m_Shader;

        //光照参数
        glm::vec3 m_LightPos;//光源位置
        glm::vec3 m_LightColor;//光源颜色
        glm::vec3 m_ObjectColor;//物体颜色
        float m_AmbientStrength;//环境光强度
        float m_SpecularStrength;//镜面光强度
        int m_Shininess; //高光系数

        glm::vec3 m_CameraPos;//相机位置
        glm::vec3 m_ObjectRotation;//物体旋转

        bool m_AutoRotateO;//物体自动旋转
        bool m_AutoMoveL;//光源自动移动

    };
}