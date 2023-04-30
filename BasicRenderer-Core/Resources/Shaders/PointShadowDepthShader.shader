#shader vertex
#version 460 core
layout(location = 0) in vec3 a_Pos;
layout(location = 5) in mat4 a_InstancedModelMatrix;

uniform mat4 u_model;
uniform bool u_isInstanced;

void main()
{
    !u_isInstanced ? gl_Position = u_model * vec4(a_Pos, 1.0): gl_Position = a_InstancedModelMatrix * vec4(a_Pos, 1.0);
}

#shader geometry
#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 cubeMatrices[6];

out vec4 FragPos; 

void main()
{
    for (int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for (int i = 0; i < 3; ++i) // for each triangle vertex
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = cubeMatrices[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}

#shader fragment
#version 460 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - lightPos);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;

    // write this as modified depth
    gl_FragDepth = lightDistance;
}