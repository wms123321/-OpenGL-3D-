#pragma once


class Application
{
public:
	static Application* getInstance();

	~Application();

	void test();



private:
	static Application* mInstance;


	Application();


};