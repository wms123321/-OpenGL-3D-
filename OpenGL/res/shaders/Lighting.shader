#shader vertex
#version 330 core

layout(location = 0) in vec3 position;//����λ��
layout(location = 1) in vec3 normal;//���㷨����

out vec3 v_FragPos;
out vec3 v_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    //����Ƭ��������ռ��е�λ��
    v_FragPos = vec3(u_Model * vec4(position, 1.0));
    //��������ռ��еķ�����
    v_Normal = mat3(transpose(inverse(u_Model))) * normal;
    //���㶥���ڲü��ռ��е�λ��
    gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);
}

#shader fragment
#version 330 core

in vec3 v_FragPos;
in vec3 v_Normal;

out vec4 color;

uniform vec3 u_LightPos;//��Դλ��
uniform vec3 u_LightColor;//��Դ��ɫ
uniform vec3 u_ObjectColor;//������ɫ
uniform vec3 u_ViewPos;//���λ��
uniform float u_AmbientStrength;//������ǿ��
uniform float u_SpecularStrength;//�����ǿ��
uniform int u_Shininess;//�߹�ϵ��

void main()
{
    //��������
    vec3 ambient = u_AmbientStrength * u_LightColor;//������=ǿ��*��Դ��ɫ
    
    //���������
    vec3 norm = normalize(v_Normal);//��һ��������
    vec3 lightDir = normalize(u_LightPos - v_FragPos);//������߷���
    float diff = max(dot(norm, lightDir), 0.0);//���㷨��������߷���ļн�����ֵ
    vec3 diffuse = diff * u_LightColor;//�������=�н�ϵ��*��Դ��ɫ
    
    //�������
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);//�۲췽��
    vec3 reflectDir = reflect(-lightDir, norm);//������߷���
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);//�߹�ǿ�� = (�۲췽���뷴�䷽��н�����)^�߹�ϵ��
    vec3 specular = u_SpecularStrength * spec * u_LightColor;//�����=ǿ��*�߹�ǿ��*��Դ��ɫ
    
    //������ɫ��������+�������+����⣬�ٳ������屾����ɫ
    vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
    color = vec4(result, 1.0);//���������ɫ��͸����Ϊ1
}