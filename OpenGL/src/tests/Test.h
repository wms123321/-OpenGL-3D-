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
		//每帧更新
		virtual void OnUpdate(float deltaTime){}
		//OpenGL渲染
		virtual void OnRender() {}
		//ImGui界面渲染
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
			//添加键值对：名称+创建T实例的lambda函数
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}


	private:
		Test*& m_CurrentTest;//指向当前激活测试的指针的引用
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;//存储测试名称和对应的创建函数

	};

}