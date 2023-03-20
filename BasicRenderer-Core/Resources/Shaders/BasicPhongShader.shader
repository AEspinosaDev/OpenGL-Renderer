#shader vertex
#version 460 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;

out vec3 pos;
out vec2 texCoord;
out mat3 TBN;
out vec3 normal;
out vec4 pos_lightSpace;


uniform mat4 u_Model;
uniform mat4 u_modelView;
uniform mat4 u_modelViewProj;
uniform mat4 u_lightViewProj;


uniform float u_TileU;
uniform float u_TileV;

void main()
{
	pos = (u_modelView * vec4(a_Pos, 1.0)).xyz;
	pos_lightSpace = u_lightViewProj * (u_Model * vec4(a_Pos, 1.0));

	vec3 T = normalize(vec3(u_modelView * vec4(a_Tangent, 0.0)));
	vec3 N = normalize(vec3(u_modelView * vec4(a_Normal, 0.0)));
	vec3 B = cross(N, T);

	TBN = mat3(T, B, N);

	normal = mat3(transpose(inverse(u_modelView))) * a_Normal;


	texCoord = a_TexCoord;
	texCoord.x *= u_TileU;
	texCoord.y *= u_TileV;

	gl_Position = u_modelViewProj * vec4(a_Pos, 1.0f);

}

#shader fragment
#version 460 core

out vec4 FragColor;

in vec3 pos;
in vec2 texCoord;
in mat3 TBN;
in vec3 normal;
in vec4 pos_lightSpace;

//Lights
struct PointLight {
	vec3 pos;
	vec3 color;
	float intensity;
	float constant;
	float linear;
	float quadratic;
};

struct DirectionalLight {
	vec3 dir;
	vec3 color;
	float intensity;
};

struct SpotLight {
	vec3 pos;
	vec3 dir;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
};

const int MAX_POINT_LIGHTS = 36;
const int MAX_DIRECTIONAL_LIGHTS = 36;
const int MAX_SPOT_LIGHTS = 36;

struct Material {
	sampler2D colorTex;
	sampler2D normalTex;
	sampler2D specularTex;
	float glossTex;
};

//Uniforms

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform int pointsLightsNumber;
uniform int directionalLightsNumber;
uniform int spotLightsNumber;

uniform Material material;

uniform sampler2D shadowMap;


vec3 N;


vec3 color = vec3(1, 1, 1);


float computeAttenuation(vec3 lightPos, float lin, float quad) {
	float d = length(lightPos - pos);
	float constant = 1.0f;

	return 1.0 / (constant + lin * d + quad * (d * d));

}

float computeShadow(vec3 lightDir) {

	// perform perspective divide
	vec3 projCoords = pos_lightSpace.xyz / pos_lightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;

	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// check whether current frag pos is in shadow
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	bias = 0.005;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	if (projCoords.z > 1.0)
		shadow = 0.0;

	return shadow;


}

vec3 shadePointLight(vec3 lightPos, vec3 color, float intensity) {

	//Diffuse
	vec3 L = normalize(lightPos - pos);
	vec3 diffuse =  max(dot(L, N), 0.0) *texture(material.colorTex, texCoord).rgb ;

	//Specular
	vec3 V = normalize(-pos);
	vec3 R = normalize(reflect(-L, N));
	float factor = max(dot(R, V), 0.0);
	vec3 specular = pow(factor, 10)*texture(material.colorTex, texCoord).rgb ;

	//Attenuation
	float attenuation = computeAttenuation(lightPos,0.022f, 0.019f);
	diffuse *= attenuation;
	specular *= attenuation;

	//Shadow 
	float shadow = computeShadow(L);

	vec3 result = (1.0 - shadow) *(diffuse + specular) * color;

	result *= intensity;
	return result;

}
vec3 shadeDirectionalLight(vec3 lightDir, vec3 color, float intensity) {

	//Diffuse
	vec3 L = normalize(lightDir);
	vec3 diffuse = max(dot(L, N), 0.0) * texture(material.colorTex, texCoord).rgb;

	//Specular
	vec3 V = normalize(-pos);
	vec3 R = normalize(reflect(-L, N));
	float factor = max(dot(R, V), 0.0);
	vec3 specular = pow(factor, 10) * texture(material.colorTex, texCoord).rgb;


	vec3 result = (diffuse + specular) * color;
	result *= intensity;
	return result;

}

vec3 shade() {

	vec3 result = vec3(0.0);


	for (int i = 0; i < pointsLightsNumber; i++) {
		result += shadePointLight(pointLights[i].pos, pointLights[i].color, pointLights[i].intensity);
	}
	for (int i = 0; i < directionalLightsNumber; i++) {
		result += shadeDirectionalLight(directionalLights[i].dir, directionalLights[i].color, directionalLights[i].intensity);
	}
	for (int i = 0; i < spotLightsNumber; i++) {
		//result += shadePointLight(spotLights[i].pos, spotLights[i].color, spotLights[i].intensity);
	}

	return result;

}

void main()
{
	//material.glossTex = 10;
	vec3 normalK = texture(material.normalTex, texCoord).rgb * 2.0 - 1.0;
	N = normalize(TBN * normalK);
	//N = normal;

	FragColor = vec4(shade(), 1.0);

}

