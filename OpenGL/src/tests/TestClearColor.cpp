#include"TestClearColor.h"
#include"Renderer.h"
#include"imgui/imgui.h"

namespace test {
	TestClearColor::TestClearColor()
		:m_ClaerColor{0.2f,0.3f,0.8f,1.0f}
	{
	}
	TestClearColor::~TestClearColor()
	{
	}
	void TestClearColor::OnUpdate(float deltaTime)
	{
	}
	void TestClearColor::OnRender()
	{
		GLCall(glClearColor(m_ClaerColor[0], m_ClaerColor[1], m_ClaerColor[2], m_ClaerColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}
	void TestClearColor::OnImGuiRender()
	{
		//ImGuiÑÕÉ«±à¼­Æ÷
		ImGui::ColorEdit4("Clear Color", m_ClaerColor);
	}
}