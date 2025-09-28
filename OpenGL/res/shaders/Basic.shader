#shader vertex
#version 330 core

layout(location = 0) in vec4 position;//����λ������
layout(location = 1) in vec2 texCoord;//������������

out vec2 v_TexCoord;//�����������

uniform mat4 u_MVP;//MVP����

void main()
{
    gl_Position = u_MVP*position;//��������任
    v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

//����Ӷ�����ɫ�����ݹ����Ĳ�ֵ�����
in vec2 v_TexCoord;

//�������������ɫ��֡������
layout(location = 0) out vec4 color;

//uniform vec4 u_Color;
uniform sampler2D u_Texture;//2D���������

void main()
{
    //ʹ�ò�����u_Texture����������v_TexCoord���������л�ȡ��ɫ
    vec4 texColor = texture(u_Texture,v_TexCoord);
    color = texColor;
    //color = u_Color;
};