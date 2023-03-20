#shader vertex
#version 460 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_texCoord;

uniform mat4 u_proj;

out vec2 texCoord;

void main() {

	texCoord = a_texCoord;

	gl_Position = u_proj * vec4(a_Pos, 1.0);

}

#shader fragment
#version 460 core

in vec2 texCoord;

uniform sampler2D vignetteTex;

out vec4 fragColor;

void main() {

	fragColor = vec4(texture(vignetteTex,texCoord).rgb, 1.0);
}

