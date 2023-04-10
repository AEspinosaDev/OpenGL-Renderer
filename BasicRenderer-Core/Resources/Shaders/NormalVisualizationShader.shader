#shader vertex
#version 460 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;

out VS_OUT{
	vec3 normal;
} vs_out;

uniform mat4 u_modelView;

void main()
{
	vs_out.normal = mat3(transpose(inverse(u_modelView))) * a_Normal;
	gl_Position = u_modelView * vec4(a_Pos, 1.0);
}


#shader geometry
#version 460 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT{
	vec3 normal;
} gs_in[];

const float LENGTH = 0.2;

uniform mat4 u_projection;

void generateLine(int index)
{
	gl_Position = u_projection * gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = u_projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * LENGTH);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	generateLine(0);
	generateLine(1);
	generateLine(2);
}




#shader fragment
#version 460 core

out vec4 fragColor;

void main(){
	fragColor = vec4(0.0,1.0,0.0,1.0);
}
