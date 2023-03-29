#shader vertex
#version 460 core

layout(location = 0) in vec3 a_Pos;

out vec3 texCoord;

uniform mat4 u_viewProj;


void main()
{
    texCoord = a_Pos;
    vec4 pos = u_viewProj * vec4(a_Pos, 1.0);
    gl_Position = pos.xyww;
}

#shader fragment
#version 460 core

out vec4 fragColor;

in vec3 texCoord;

uniform samplerCube u_cubeMapText;

void main()
{
    fragColor = texture(u_cubeMapText, texCoord);
}