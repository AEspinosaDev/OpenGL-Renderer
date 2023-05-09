#shader vertex
#version 460 core

layout(location = 0) in vec3 a_Pos;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 5) in mat4 a_InstancedModelMatrix;

uniform mat4 u_modelViewProj;
uniform mat4 u_viewProj;
uniform mat4 u_model;


uniform float u_TileU;
uniform float u_TileV;
uniform bool u_isInstanced;

out vec2 texCoord;

void main() {

	texCoord = a_TexCoord;
	texCoord.x *= u_TileU;
	texCoord.y *= u_TileV;

	!u_isInstanced ? gl_Position = u_modelViewProj * vec4(a_Pos, 1.0) : gl_Position = (u_viewProj * (u_model * a_InstancedModelMatrix)) * vec4(a_Pos, 1.0);
}

#shader fragment
#version 460 core

in vec2 texCoord;

uniform sampler2D u_colorTex;
uniform sampler2D u_opacityTex;
uniform float u_opacity;
uniform vec3 u_color;
uniform bool u_hasOpacityTex;
uniform bool u_hasColorTex;

out vec4 fragColor;

void main() {

	vec3 color = vec3(1.0f);
	float opacity = 1.0f;

	u_hasColorTex ? color = texture(u_colorTex, texCoord).rgb : color = u_color;
	u_hasOpacityTex ? opacity = texture(u_opacityTex, texCoord).r : opacity = u_opacity;

	fragColor = vec4(color, opacity);
}

