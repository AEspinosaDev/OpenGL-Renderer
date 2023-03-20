#shader vertex
#version 460 core

layout(location = 0) in vec3 a_Pos;

uniform mat4 u_Light_ModelViewProj;

void main()
{
   
    gl_Position = u_Light_ModelViewProj * vec4(a_Pos, 1.0);
}


#shader fragment
#version 460 core


void main()
{
    // gl_FragDepth = gl_FragCoord.z;
}