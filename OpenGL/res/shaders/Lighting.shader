#shader vertex
#version 330 core

layout(location = 0) in vec3 position;//顶点位置
layout(location = 1) in vec3 normal;//顶点法向量

out vec3 v_FragPos;
out vec3 v_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    //计算片段在世界空间中的位置
    v_FragPos = vec3(u_Model * vec4(position, 1.0));
    //计算世界空间中的法向量
    v_Normal = mat3(transpose(inverse(u_Model))) * normal;
    //计算顶点在裁剪空间中的位置
    gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);
}

#shader fragment
#version 330 core

in vec3 v_FragPos;
in vec3 v_Normal;

out vec4 color;

uniform vec3 u_LightPos;//光源位置
uniform vec3 u_LightColor;//光源颜色
uniform vec3 u_ObjectColor;//物体颜色
uniform vec3 u_ViewPos;//相机位置
uniform float u_AmbientStrength;//环境光强度
uniform float u_SpecularStrength;//镜面光强度
uniform int u_Shininess;//高光系数

void main()
{
    //环境光照
    vec3 ambient = u_AmbientStrength * u_LightColor;//环境光=强度*光源颜色
    
    //漫反射光照
    vec3 norm = normalize(v_Normal);//归一化法向量
    vec3 lightDir = normalize(u_LightPos - v_FragPos);//计算光线方向
    float diff = max(dot(norm, lightDir), 0.0);//计算法向量与光线方向的夹角余弦值
    vec3 diffuse = diff * u_LightColor;//漫反射光=夹角系数*光源颜色
    
    //镜面光照
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);//观察方向
    vec3 reflectDir = reflect(-lightDir, norm);//反射光线方向
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);//高光强度 = (观察方向与反射方向夹角余弦)^高光系数
    vec3 specular = u_SpecularStrength * spec * u_LightColor;//镜面光=强度*高光强度*光源颜色
    
    //最终颜色：环境光+漫反射光+镜面光，再乘以物体本身颜色
    vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
    color = vec4(result, 1.0);//输出最终颜色，透明度为1
}