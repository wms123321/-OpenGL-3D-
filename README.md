# OpenGL 3D 渲染学习项目

## 项目概述

本项目是一个基于 OpenGL 3.3 核心模式的 3D 渲染学习实践项目，旨在深入理解计算机图形学基础原理与渲染流程。

## 功能特点

- 支持 2D 纹理加载与渲染，实现透明通道混合效果
- 基于 GLM 数学库实现 3D 模型的矩阵变换（模型、视图、投影矩阵）
- 集成 Phong 光照模型，实现环境光、漫反射光与镜面反射光效果
- 通过 ImGui 构建交互式 UI 界面，支持实时切换渲染场景（纯色清除、2D 纹理、3D 模型、光照效果）及调整参数（颜色、光照强度等）
- 封装 OpenGL 基础组件（顶点数组、缓冲对象等），简化渲染流程

## 技术栈

- 编程语言：C++
- 图形 API：OpenGL 3.3（核心模式）
- 辅助库：
  - GLEW：OpenGL 扩展管理
  - GLFW：窗口创建与输入处理
  - GLM：3D 数学运算库
  - ImGui：交互式图形界面



## 使用方法

运行可执行文件，通过 ImGui 界面交互：

- 切换不同渲染场景
- 调整光照参数、颜色等配置
- 实时观察渲染效果变化

## 项目结构

```plaintext
OpenGL/
├─Dependencies  # 第三方依赖库
│  ├─GLEW
│  │
│  └─GLFW
├─res          # 资源文件目录
│  ├─shaders   # GLSL 着色器文件
│  │      3D.shader
│  │      Basic.shader
│  │      Lighting.shader
│  │
│  └─textures  # 纹理图片
├─src          # 源代码目录
│  │  Application.cpp
│  │  IndexBuffer.cpp
│  │  IndexBuffer.h
│  │  Renderer.cpp
│  │  Renderer.h
│  │  Shader.cpp
│  │  Shader.h
│  │  Texture.cpp
│  │  Texture.h
│  │  VertexArray.cpp
│  │  VertexArray.h
│  │  VertexBuffer.cpp
│  │  VertexBuffer.h
│  │  VertexBufferLayout.h
│  │
│  ├─tests       # 测试场景
│  │      Test.cpp
│  │      Test.h
│  │      Test3DCube.cpp
│  │      Test3DCube.h
│  │      TestClearColor.cpp
│  │      TestClearColor.h
│  │      TestLighting.cpp
│  │      TestLighting.h
│  │      TestTexture2D.cpp
│  │      TestTexture2D.h
│  │
│  └─vendor     #第三方依赖库源代码
│      ├─glm 
│      │
│      ├─imgui
│      │
│      └─stb_image
└── README.md    # 项目说明文档

```