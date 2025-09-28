#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<assert.h>
#include<fstream>
#include<string>
#include<sstream>

#include"Renderer.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"VertexArray.h"
#include"Shader.h"
#include"VertexBufferLayout.h"
#include"Texture.h"
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"imgui/imgui.h"
#include"imgui/imgui_impl_glfw.h"
#include"imgui/imgui_impl_opengl3.h"
#include"tests/TestClearColor.h"
#include"tests/TestTexture2D.h"
#include"tests/Test3DCube.h"
#include"tests/TestLighting.h"

//��Ӧ����仯����
void SizeCallBack(GLFWwindow* windows, int w, int h)
{
    std::cout << "�����´�С��" << w << "," << h << std::endl;
    glViewport(0, 0, w, h);
}


//���̻ص�
void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mod)
{
   
    std::cout << "������" << (char)key <<"," <<"action:"<<action<<"," << "mod:" << mod << std::endl;
}


int main(void)
{
   



    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //3.3����ģʽ
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640*2, 480*2, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    //���ô�ֱͬ��
    glfwSwapInterval(1);


    //��������
    glfwSetFramebufferSizeCallback(window, SizeCallBack);

    ////��������
    //glfwSetKeyCallback(window, KeyCallBack);


    if (glewInit() != GLEW_OK)
    {
        std::cout << "error !" << std::endl;
    }

    std::cout << "OpenGL�汾��" << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL�汾��" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    {
        //��ϣ�����͸��
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     //���ü��̿���

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

       
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");


        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        //ע�����
        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
        testMenu->RegisterTest<test::Test3DCube>("3D Cube");
        testMenu->RegisterTest<test::TestLighting>("Lighting");

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            renderer.Clear();
 
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest)
            {
                currentTest->OnUpdate(0.001f);
                currentTest->OnRender();

                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            ImGui::Render();

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
        
    }

    //����ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}