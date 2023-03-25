#shader vertex
#version 460 core

layout(location = 0) in vec3 a_Pos;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_modelViewProj;

uniform float u_TileU;
uniform float u_TileV;

out vec2 texCoord;

void main() {
	gl_Position = u_modelViewProj * vec4(a_Pos, 1.0);

	texCoord = a_TexCoord;
	texCoord.x *= u_TileU;
	texCoord.y *= u_TileV;
}

#shader fragment
#version 460 core

in vec2 texCoord;

uniform vec3 u_color;
uniform sampler2D u_colorTex;

out vec4 fragColor;

void main() {

	fragColor = vec4(u_color, 1.0);
}

