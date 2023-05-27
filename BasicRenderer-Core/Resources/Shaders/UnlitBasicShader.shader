#shader vertex
#version 460 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;

layout(location = 2) in vec2 a_TexCoord;
layout(location = 5) in mat4 a_InstancedModelMatrix;

uniform mat4 u_modelViewProj;
uniform mat4 u_viewProj;
uniform mat4 u_model;
uniform mat4 u_modelView;
uniform mat4 u_proj;



uniform float u_TileU;
uniform float u_TileV;
uniform bool u_isInstanced;

uniform bool u_isOutlining;

out vec2 texCoord;



void main() {

	texCoord = a_TexCoord;
	texCoord.x *= u_TileU;
	texCoord.y *= u_TileV;

	vec3 pos;

	pos = a_Pos; /*+normalize(a_Normal) * 0.5f;*/

	// mat4 sizeMat = u_model;
	//sizeMat[0][0] = 1.1f;
	//sizeMat[1][1] = 1.1f;
	//sizeMat[2][2] = 1.1f;

	!u_isInstanced ? gl_Position = (u_proj * u_modelView) * vec4(pos, 1.0) : gl_Position = (u_viewProj * (u_model * a_InstancedModelMatrix)) * vec4(pos, 1.0);
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

