#shader vertex
#version 460 core

layout(location = 0) in vec3 a_Pos;

uniform mat4 u_modelViewProj;

void main() {
	gl_Position = u_modelViewProj * vec4(a_Pos, 1.0);
}

#shader fragment
#version 460 core

uniform vec3 u_color;

out vec4 fragColor;

void main() {
	fragColor = vec4(u_color, 1.0);
}

