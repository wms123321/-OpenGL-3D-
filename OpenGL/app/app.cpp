#include"app.h"
#include<iostream>


Application* Application::mInstance = nullptr;

Application* Application::getInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new Application();
	}
	return mInstance;
}

void Application::test()
{
	std::cout << "app test !" << std::endl;
}


Application::Application()
{

}

Application::~Application()
{

}
