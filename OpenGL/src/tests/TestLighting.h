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

        //���ղ���
        glm::vec3 m_LightPos;//��Դλ��
        glm::vec3 m_LightColor;//��Դ��ɫ
        glm::vec3 m_ObjectColor;//������ɫ
        float m_AmbientStrength;//������ǿ��
        float m_SpecularStrength;//�����ǿ��
        int m_Shininess; //�߹�ϵ��

        glm::vec3 m_CameraPos;//���λ��
        glm::vec3 m_ObjectRotation;//������ת

        bool m_AutoRotateO;//�����Զ���ת
        bool m_AutoMoveL;//��Դ�Զ��ƶ�

    };
}