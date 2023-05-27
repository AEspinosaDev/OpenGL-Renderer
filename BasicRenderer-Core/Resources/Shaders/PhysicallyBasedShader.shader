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
	samplerCube shadowMap;
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

//Material interface
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
	int			presetType;

};

//Constants
const float PI = 3.14159265359;
const int MAX_LIGHTS = 12;


//Uniforms
//uniform mat4 u_Model;
uniform Material material;
uniform PointLight pointLights[3];
uniform DirectionalLight directionalLights[MAX_LIGHTS];
//uniform SpotLight spotLights[MAX_LIGHTS];
uniform int pointsLightsNumber;
uniform int directionalLightsNumber;
//uniform int spotLightsNumber;
uniform float u_shadowsFarPlane;
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

float computeAttenuation(vec3 lightPos, float lin, float quad) {
	float d = length(lightPos - pos);
	float constant = 1.0f;

	return 1.0 / (constant + lin * d + quad * (d * d));

}
float computeShadow(sampler2D shadowMap, mat4 lightViewProj, vec3 lightDir) {

	vec4 pos_lightSpace = lightViewProj * vec4(modelPos, 1.0);

	// perform perspective divide

	vec3 projCoords = pos_lightSpace.xyz / pos_lightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;

	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// check whether current frag pos is in shadow
	float bias = max(0.0005 * (1.0 - dot(normal, lightDir)), 0.00005);
	//bias = 0.005;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	if (projCoords.z > 1.0)
		shadow = 0.0;


	return shadow;
	//return lightDir.x;


}

float computePointShadow(samplerCube shadowMap, vec3 lightPos)
{
	// get vector between fragment position and light position
	vec3 fragToLight = modelPos - lightPos;
	// use the light to fragment vector to sample from the depth map    
	float closestDepth = texture(shadowMap, fragToLight).r;
	// it is currently in linear range between [0,1]. Re-transform back to original value
	closestDepth *= u_shadowsFarPlane;
	// now get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight);
	// now test for shadows
	//float bias = 0.0; //for now
	float bias = max(0.005 * (1.0 - dot(normal, fragToLight)), 0.0005);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	// discard shadowing fragments further away
	if (currentDepth > u_shadowsFarPlane) shadow = 0.0;


	return shadow;
}

//Fresnel
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

//Normal Distribution 
//Trowbridge - Reitz GGX
float distributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return num / denom;
}
//Geometry
//Schlick - GGX
float geometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = geometrySchlickGGX(NdotV, roughness);
	float ggx1 = geometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}


vec3 computePBRLighting(vec3 L, vec3 color, float intensity, float attenuation) {

	//Heuristic fresnel factor
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metalness);

	//Radiance
	vec3 V = normalize(-pos);
	vec3 H = normalize(V + L);

	vec3 radiance = color * attenuation * intensity;

	// Cook-Torrance BRDF
	float NDF = distributionGGX(N, H, roughness);
	float G = geometrySmith(N, V, L, roughness);
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
	vec3 partialResult;
	float shadow;

	for (int i = 0; i < pointsLightsNumber; i++) {
		vec3 lightVector = normalize(pointLights[i].pos - pos);
		float attenuation = computeAttenuation(pointLights[i].pos, 0.022f, 0.019f);
		partialResult = computePBRLighting(lightVector, pointLights[i].color, pointLights[i].intensity, attenuation);
		//Point perspective shadow 
		material.receiveShadows ? shadow = computePointShadow(pointLights[i].shadowMap, pointLights[i].worldPos) : shadow = 0.0;
		partialResult *= (1.0 - shadow);
		result += partialResult;
	}
	for (int i = 0; i < directionalLightsNumber; i++) {
		partialResult = computePBRLighting(normalize(directionalLights[i].dir), directionalLights[i].color, directionalLights[i].intensity, 1.0f);
		//Standard shadow 
		material.receiveShadows ? shadow = computeShadow(directionalLights[i].shadowMap, directionalLights[i].lightViewProj, normalize(directionalLights[i].dir)) : shadow = 0.0;
		partialResult *= (1.0 - shadow);
		result += partialResult;
	}
	//for (int i = 0; i < spotLightsNumber; i++) {
	//	result += shadePointLight(spotLights[i].pos, spotLights[i].color, spotLights[i].intensity);
	//}

	//Ambient component
	vec3 ambient = (u_ambientColor * u_ambientStrength) * albedo * ao;
	result += ambient;

	//Tone Up
	result = result / (result + vec3(1.0));
	//Gamma Correction
	//result = pow(result, vec3(1.0 / 2.2));

	return result;

}

void main()
{

	material.hasNormalTex ? N = normalize(TBN * (texture(material.normalTex, texCoord).rgb * 2.0 - 1.0)) : N = normal;
	material.hasAlbedoTex ? albedo = texture(material.albedoTex, texCoord).rgb : albedo = material.albedo;
	//material.hasAlbedoTex ? albedo = pow(texture(material.albedoTex, texCoord).rgb, vec3(2.2)) : albedo = pow(material.albedo, vec3(2.2));
	material.hasOpacityTex ? opacity = texture(material.opacityTex, texCoord).r : opacity = material.opacity;
	//Discriminate by preset type
	if (material.presetType == 0) {
		material.hasRoughnessTex ? roughness = texture(material.roughnessTex, texCoord).r : roughness = material.roughness;
		material.hasMetalnessTex ? metalness = texture(material.metalnessTex, texCoord).r : metalness = material.metalness;
		material.hasAOTex ? ao = texture(material.AOTex, texCoord).r : ao = material.ao;
	}
	else if (material.presetType == 1) {
		//Unity HDRP uses glossiness not roughness pipeline, so it has to be inversed
		roughness = 1.0 - pow(texture(material.roughnessTex, texCoord).a, 2.2);
		metalness = pow(texture(material.roughnessTex, texCoord).r, 2.2);
		ao = pow(texture(material.roughnessTex, texCoord).g, 2.2);
	}
	else if (material.presetType == 2) {
		roughness = pow(texture(material.roughnessTex, texCoord).g, 2.2);
		metalness = pow(texture(material.roughnessTex, texCoord).b, 2.2);
		ao = pow(texture(material.roughnessTex, texCoord).r, 2.2);
	}

	FragColor = vec4(shade(), opacity);
	//FragColor = vec4(1.0, 0.0, 0.0, opacity);

}

