#shader vertex
#version 460 core


layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;

out vec3 pos;
out vec3 modelPos;
out vec2 texCoord;
out mat3 TBN;
out vec3 normal;



uniform mat4 u_Model;
uniform mat4 u_modelView;
uniform mat4 u_modelViewProj;


uniform float u_TileU;
uniform float u_TileV;

void main()
{
	pos = (u_modelView * vec4(a_Pos, 1.0)).xyz;
	modelPos = (u_Model * vec4(a_Pos, 1.0)).xyz;


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

const int MAX_LIGHTS = 32;

out vec4 FragColor;

in vec3 pos;
in vec3 modelPos;
in vec2 texCoord;
in mat3 TBN;
in vec3 normal;
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

	bool castShadows;
	sampler2D shadowMap;
	mat4 lightViewProj;
};

struct Material {

	bool hasColorTex;
	bool hasSpecularTex;
	bool hasGlossTex;
	bool hasNormalTex;
	bool hasOpacityTex;

	sampler2D colorTex;
	sampler2D normalTex;
	sampler2D specularTex;
	sampler2D glossTex;
	sampler2D opacityTex;

	float opacity;
	vec3 baseColor;
	float specularity;
	float shininess;
	bool receiveShadows;

};

//Uniforms
uniform mat4 u_Model;
uniform Material material;
uniform PointLight pointLights[2];
uniform DirectionalLight directionalLights[MAX_LIGHTS];
uniform SpotLight spotLights[MAX_LIGHTS];
uniform int pointsLightsNumber;
uniform int directionalLightsNumber;
uniform int spotLightsNumber;
uniform float u_ambientStrength;
uniform vec3 u_ambientColor;
uniform samplerCube u_skybox;

//Surface properties
vec3 N;
vec3 albedo;
float specularity;
float opacity;
float shininess;


vec3 color = vec3(1, 1, 1);


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
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	bias = 0.005;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	if (projCoords.z > 1.0)
		shadow = 0.0;

	return shadow;
	//return lightDir.x;


}

float computePointShadow(samplerCube shadowMap ,vec3 lightPos)
{
	// get vector between fragment position and light position
	vec3 fragToLight = modelPos - lightPos;
	// use the light to fragment vector to sample from the depth map    
	float closestDepth = texture(shadowMap, fragToLight).r;
	// it is currently in linear range between [0,1]. Re-transform back to original value
	closestDepth *= 25.0;
	// now get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight);
	// now test for shadows
	float bias = 0.05;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	if (currentDepth > 25.0) shadow = 0.0;
	return shadow;
}

vec3 shadePointLight(vec3 lightPos, vec3 color, float intensity, samplerCube shadowMap, vec3 worldPos, bool castShadows) {

	//Diffuse
	vec3 L = normalize(lightPos - pos);
	vec3 diffuse = max(dot(L, N), 0.0) * color;

	//Specular
	vec3 V = normalize(-pos);
	vec3 R = normalize(reflect(-L, N));
	float factor = max(dot(R, V), 0.0);
	vec3 specular = pow(factor, shininess) * specularity * color;

	//Attenuation
	float attenuation = computeAttenuation(lightPos, 0.022f, 0.019f);
	diffuse *= attenuation;
	specular *= attenuation;

	//Shadow 
	float shadow;
	//material.receiveShadows ? shadow = computeShadow(shadowMap, lightViewProj, L) : shadow = 0.0;
	material.receiveShadows ? shadow = computePointShadow(shadowMap, worldPos) : shadow = 0.0;

	vec3 result = (1.0 - shadow) * (diffuse + specular) * albedo;
	result *= intensity;
	return result;

}
vec3 shadeDirectionalLight(vec3 lightDir, vec3 color, float intensity, sampler2D shadowMap, mat4 lightViewProj, bool castShadows) {

	//Diffuse
	vec3 L = normalize(lightDir);
	vec3 diffuse = max(dot(L, N), 0.0) * color;

	//Specular
	vec3 V = normalize(-pos);
	vec3 R = normalize(reflect(-L, N));
	float factor = max(dot(R, V), 0.0);
	vec3 specular = pow(factor, shininess) * specularity *color;


	//Shadow 
	float shadow;
	castShadows ? shadow = computeShadow(shadowMap, lightViewProj, L) : shadow = 0.0;

	vec3 result = (1.0 - shadow) * (diffuse + specular) * albedo;
	result *= intensity;
	return result;

}
vec3 shadeAmbientLight() {
	return albedo * u_ambientColor * u_ambientStrength;
}

vec3 shade() {

	vec3 result = vec3(0.0);

	//Just ambient
	result += shadeAmbientLight();

	for (int i = 0; i < pointsLightsNumber; i++) {
		result += shadePointLight(pointLights[i].pos, pointLights[i].color, pointLights[i].intensity, pointLights[i].shadowMap, pointLights[i].worldPos,
			pointLights[i].castShadows);
	}
	for (int i = 0; i < directionalLightsNumber; i++) {
		result += shadeDirectionalLight(directionalLights[i].dir, directionalLights[i].color, directionalLights[i].intensity,
			directionalLights[i].shadowMap, directionalLights[i].lightViewProj, directionalLights[i].castShadows);
	}
	//for (int i = 0; i < spotLightsNumber; i++) {
	//	result += shadePointLight(spotLights[i].pos, spotLights[i].color, spotLights[i].intensity);
	//}

	return result;

}

void main()
{

	material.hasNormalTex ? N = normalize(TBN * (texture(material.normalTex, texCoord).rgb * 2.0 - 1.0)) : N = normal;
	material.hasColorTex ? albedo = texture(material.colorTex, texCoord).rgb : albedo = material.baseColor;
	material.hasSpecularTex ? specularity = texture(material.specularTex, texCoord).r : specularity = material.specularity;
	material.hasGlossTex ? shininess = texture(material.glossTex, texCoord).r : shininess = material.shininess;
	material.hasOpacityTex ? opacity = texture(material.opacityTex, texCoord).r : opacity = material.opacity;

	vec3 r = normalize(reflect(normalize(pos), N));
	vec3 result = mix(shade(), vec3(texture(u_skybox, r).rgb), 0.0);

	FragColor = vec4(result, opacity);

}

