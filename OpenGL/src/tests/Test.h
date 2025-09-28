#pragma once

#include<functional>
#include<vector>
#include<string>
#include<iostream>
namespace test {

	class Test
	{
	public:
		Test() {};
		virtual ~Test() {};
		//ÿ֡����
		virtual void OnUpdate(float deltaTime){}
		//OpenGL��Ⱦ
		virtual void OnRender() {}
		//ImGui������Ⱦ
		virtual void OnImGuiRender() {}

	private:

	};

	class TestMenu :public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);
		void OnImGuiRender()override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Register test :" << name << std::endl;
			//��Ӽ�ֵ�ԣ�����+����Tʵ����lambda����
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}


	private:
		Test*& m_CurrentTest;//ָ��ǰ������Ե�ָ�������
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;//�洢�������ƺͶ�Ӧ�Ĵ�������

	};

}