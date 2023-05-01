#shader vertex
#version 460 core

layout(location = 0) in vec3 a_Pos;
layout(location = 5) in mat4 a_InstancedModelMatrix;

uniform mat4 u_Light_ModelViewProj;
uniform mat4 u_viewProj;
uniform mat4 u_model;
//uniform sample2D alphaMask;

uniform bool u_isInstanced;

void main()
{
    !u_isInstanced ? gl_Position = u_Light_ModelViewProj * vec4(a_Pos, 1.0) : gl_Position = (u_viewProj * (u_model *a_InstancedModelMatrix)) * vec4(a_Pos, 1.0);
}


#shader fragment
#version 460 core


void main()
{
    // gl_FragDepth = gl_FragCoord.z;
}