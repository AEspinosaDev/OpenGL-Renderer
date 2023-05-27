#shader vertex
#version 460 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 5) in mat4 a_InstancedModelMatrix;

out VS_OUT{
	vec3 normal;
	vec3 tangent;
} vs_out;

uniform mat4 u_modelView;
uniform bool u_isInstanced;

void main()
{
	if (!u_isInstanced) {
		vs_out.normal = mat3(transpose(inverse(u_modelView))) * a_Normal;
		vs_out.tangent = mat3(transpose(inverse(u_modelView))) * a_Tangent;
		gl_Position = u_modelView * vec4(a_Pos, 1.0);
	}
	else {
		vs_out.normal = mat3(transpose(inverse(u_modelView * a_InstancedModelMatrix))) * a_Normal;
		vs_out.tangent = mat3(transpose(inverse(u_modelView * a_InstancedModelMatrix))) * a_Tangent;
		gl_Position = (u_modelView * a_InstancedModelMatrix) * vec4(a_Pos, 1.0);
	}
}


#shader geometry
#version 460 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT{
	vec3 normal;
	vec3 tangent;
} gs_in[];

const float LENGTH = 0.4;

uniform mat4 u_projection;
uniform bool u_renderTangent;
uniform bool u_renderNormal;

void generateLines(int index)
{
	if (u_renderNormal) {
		gl_Position = u_projection * gl_in[index].gl_Position;
		EmitVertex();
		gl_Position = u_projection * (gl_in[index].gl_Position + normalize(vec4(gs_in[index].normal, 0.0)) * LENGTH);
		EmitVertex();
		EndPrimitive();
	}
	if (u_renderTangent) {
		gl_Position = u_projection * gl_in[index].gl_Position;
		EmitVertex();
		gl_Position = u_projection * (gl_in[index].gl_Position + normalize(vec4(gs_in[index].tangent, 0.0)) * LENGTH);
		EmitVertex();
		EndPrimitive();
	}
}


void main()
{
	generateLines(0);
	generateLines(1);
	generateLines(2);
}




#shader fragment
#version 460 core

uniform bool u_renderNormal;

out vec4 fragColor;

void main() {
	if (u_renderNormal) 
	fragColor = vec4(0.0, 1.0, 0.0, 1.0);
	else
	fragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
