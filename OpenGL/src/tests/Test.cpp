#include"Test.h"
#include"imgui/imgui.h"
namespace test {
	TestMenu::TestMenu(Test*& currentTestPointer)
		:m_CurrentTest(currentTestPointer)
	{
	}

	//遍历所有注册的测试，为每个测试创建一个ImGui按钮
	void TestMenu::OnImGuiRender()
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
				m_CurrentTest = test.second();//创建测试实例的函数
		}
	}
}