#shader vertex
#version 330 core

layout(location = 0) in vec4 position;//顶点位置坐标
layout(location = 1) in vec2 texCoord;//顶点纹理坐标

out vec2 v_TexCoord;//输出纹理坐标

uniform mat4 u_MVP;//MVP矩阵

void main()
{
    gl_Position = u_MVP*position;//顶点坐标变换
    v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

//输入从顶点着色器传递过来的插值后变量
in vec2 v_TexCoord;

//输出最终像素颜色到帧缓冲区
layout(location = 0) out vec4 color;

//uniform vec4 u_Color;
uniform sampler2D u_Texture;//2D纹理采样器

void main()
{
    //使用采样器u_Texture和纹理坐标v_TexCoord，从纹理中获取颜色
    vec4 texColor = texture(u_Texture,v_TexCoord);
    color = texColor;
    //color = u_Color;
};