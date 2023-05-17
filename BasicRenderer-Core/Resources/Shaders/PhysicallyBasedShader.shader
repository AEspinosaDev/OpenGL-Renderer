#shader vertex
#version 460 core


layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Color;
layout(location = 5) in mat4 a_InstancedModelMatrix;

out vec3 pos;
out vec3 modelPos;
out vec2 texCoord;
out mat3 TBN;
out vec3 normal;

uniform bool u_isInstanced;

uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat4 u_Proj;
uniform mat4 u_modelView;
uniform mat4 u_modelViewProj;

uniform float u_TileU;
uniform float u_TileV;

vec4 computeMatrixTransformations(mat4 model, mat4 modelView, mat4 modelViewProj) {

	pos = (modelView * vec4(a_Pos, 1.0)).xyz;
	modelPos = (model * vec4(a_Pos, 1.0)).xyz;

	vec3 T = -normalize(vec3(modelView * vec4(a_Tangent, 0.0)));
	vec3 N = normalize(vec3(modelView * vec4(a_Normal, 0.0)));
	vec3 B = cross(N, T);

	TBN = mat3(T, B, N);
	normal = mat3(transpose(inverse(modelView))) * a_Normal;

	return modelViewProj * vec4(a_Pos, 1.0f);

}

void main()
{
	texCoord = a_TexCoord;
	texCoord.x *= u_TileU;
	texCoord.y *= u_TileV;

	vec4 outPosition;

	if (!u_isInstanced) {
		outPosition = computeMatrixTransformations(u_Model, u_modelView, u_modelViewProj);
	}
	else {
		mat4 modelView = u_View * (u_Model * a_InstancedModelMatrix);
		mat4 modelViewProj = u_Proj * modelView;
		outPosition = computeMatrixTransformations(u_Model * a_InstancedModelMatrix, modelView, modelViewProj);
	}

	gl_Position = outPosition;

}

#shader fragment
#version 460 core

out vec4	FragColor;

in vec3		pos;
in vec3		modelPos;
in vec2		texCoord;
in mat3		TBN;
in vec3		normal;
//in vec4 pos_lightSpace;

//Lights
struct PointLight {
	vec3 pos;
	vec3 worldPos;
	vec3 color;
	float intensity;
	float constant;
	float linear;
	float quadratic;
	float att;

	bool castShadows;
	//samplerCube shadowMap;
	mat4 lightViewProj;
};

struct DirectionalLight {
	vec3 dir;
	vec3 color;
	float intensity;

	bool castShadows;
	sampler2D shadowMap;
	mat4 lightViewProj;
};

struct SpotLight {
	vec3 pos;
	vec3 dir;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
	float att;

	bool castShadows;
	sampler2D shadowMap;
	mat4 lightViewProj;
};

struct Material {

	bool		hasAlbedoTex;
	bool		hasRoughnessTex;
	bool		hasMetalnessTex;
	bool		hasNormalTex;
	bool		hasOpacityTex;
	bool		hasAOTex;

	sampler2D	albedoTex;
	sampler2D	normalTex;
	sampler2D	roughnessTex;
	sampler2D	metalnessTex;
	sampler2D	opacityTex;
	sampler2D	AOTex;


	vec3		albedo;
	float		roughness;
	float		metalness;
	float		opacity;
	float		ao;

	bool		receiveShadows;

};

//Constants
const float PI = 3.14159265359;
const int MAX_LIGHTS = 32;


//Uniforms
//uniform mat4 u_Model;
uniform Material material;
uniform PointLight pointLights[3];
//uniform DirectionalLight directionalLights[MAX_LIGHTS];
//uniform SpotLight spotLights[MAX_LIGHTS];
uniform int pointsLightsNumber;
//uniform int directionalLightsNumber;
//uniform int spotLightsNumber;
//uniform float u_shadowsFarPlane;
uniform float u_ambientStrength;
uniform vec3 u_ambientColor;
uniform samplerCube u_skybox;

//Surface properties
vec3	N;
vec3	albedo;
float	roughness;
float	opacity;
float	metalness;
float	ao;

vec3 color = vec3(1, 1, 1);

float computeAttenuation(vec3 lightPos, float lin, float quad) {
	float d = length(lightPos - pos);
	float constant = 1.0f;

	return 1.0 / (constant + lin * d + quad * (d * d));

}



vec3 shadePointLight(vec3 lightPos, vec3 color, float intensity, samplerCube shadowMap, vec3 worldPos, bool castShadows) {

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metalness);

	//Radiance
	vec3 V = normalize(-pos);
	vec3 L = normalize(lightPos - pos);
	vec3 H = normalize(V + L);

	float distance = length(lightPos - pos);
	float attenuation = 1.0 / (distance * distance);
	vec3 radiance = color * attenuation;

	// Cook-Torrance BRDF
	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 kD = vec3(1.0) - F;
	kD *= 1.0 - metalness;

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
	vec3 specular = numerator / denominator;

	// Add to outgoing radiance result
	float lambertian = max(dot(N, L), 0.0);
	return (kD * albedo / PI + specular) * radiance * lambertian;

}

vec3 shade() {

	vec3 result = vec3(0.0);

	for (int i = 0; i < pointsLightsNumber; i++) {

		result += shadePointLight(pointLights[i].pos, pointLights[i].color, pointLights[i].intensity, pointLights[i].shadowMap, pointLights[i].worldPos,
			pointLights[i].castShadows);
	}

	//Ambient component
	vec3 ambient = (u_ambientColor*u_ambientStrength) * albedo * ao;
	result += ambient;

	//Tone Up
	result = result / (result + vec3(1.0));
	////Gamma Correction
	//result = pow(result, vec3(1.0 / 2.2));

	return result;

}

void main()
{

	//material.hasNormalTex ? N = normalize(TBN * (texture(material.normalTex, texCoord).rgb * 2.0 - 1.0)) : N = normal;
	N = normal;
	material.hasAlbedoTex ? albedo = texture(material.albedoTex, texCoord).rgb : albedo = material.albedo;
	material.hasRoughnessTex ? roughness = texture(material.roughnessTex, texCoord).r : roughness = material.roughness;
	material.hasMetalnessTex ? metalness = texture(material.metalnessTex, texCoord).r : metalness = material.metalness;
	material.hasOpacityTex ? opacity = texture(material.opacityTex, texCoord).r : opacity = material.opacity;
	material.hasAOTex ? ao = texture(material.AOTex, texCoord).r : ao = material.ao;

	FragColor = vec4(shade(), opacity);

}

