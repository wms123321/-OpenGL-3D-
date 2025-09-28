#pragma once

#include "Test.h"
#include "VertexArray.h"
#include "Shader.h"
#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"

namespace test {
    class Test3DCube : public Test
    {
    public:
        Test3DCube();
        ~Test3DCube();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        std::unique_ptr<VertexArray> m_Va;
        std::unique_ptr<VertexBuffer> m_Vb;
        std::unique_ptr<IndexBuffer> m_Ib;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;

        glm::vec3 m_Translation;//位置
        glm::vec3 m_Rotation;//旋转角度
        glm::vec3 m_Scale;//缩放
        glm::mat4 m_Proj;//投影矩阵
        glm::mat4 m_View;//视图矩阵

        float m_CameraDistance;//相机距离
        bool m_AutoRotate;//是否自动旋转
    };
}