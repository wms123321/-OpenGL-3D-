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

        glm::vec3 m_Translation;//λ��
        glm::vec3 m_Rotation;//��ת�Ƕ�
        glm::vec3 m_Scale;//����
        glm::mat4 m_Proj;//ͶӰ����
        glm::mat4 m_View;//��ͼ����

        float m_CameraDistance;//�������
        bool m_AutoRotate;//�Ƿ��Զ���ת
    };
}