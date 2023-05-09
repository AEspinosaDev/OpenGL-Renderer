#shader vertex
#version 460 core
layout(location = 0) in vec3 a_Pos;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 5) in mat4 a_InstancedModelMatrix;

uniform mat4 u_model;
uniform bool u_isInstanced;

out VS_OUT{
	vec2 texCoord;
} vs_out;

void main()
{
	vs_out.texCoord = a_TexCoord;
	!u_isInstanced ? gl_Position = u_model * vec4(a_Pos, 1.0) : gl_Position = (u_model * a_InstancedModelMatrix) * vec4(a_Pos, 1.0);
}

#shader geometry
#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 cubeMatrices[6];

in VS_OUT{
	vec2 texCoord;
} gs_in[];

out GS_OUT{
	vec2 texCoord;
} gs_out;

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
			gs_out.texCoord = gs_in[i].texCoord;
			EmitVertex();
		}
		EndPrimitive();
	}
}

#shader fragment
#version 460 core
in vec4 FragPos;

in GS_OUT{
	vec2 texCoord;
} fs_in;

uniform vec3 lightPos;
uniform float far_plane;
uniform sampler2D alphaMask;

void main()
{
	/*float op = texture(alphaMask, fs_in.texCoord).r;
	return;*/
	float lightDistance = 0;
	//if (op == 1)
		// get distance between fragment and light source
		lightDistance = length(FragPos.xyz - lightPos);
	

	// map to [0;1] range by dividing by far_plane
	lightDistance = lightDistance / far_plane;

	// write this as modified depth
	gl_FragDepth = lightDistance;
}