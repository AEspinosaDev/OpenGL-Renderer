#pragma once
#include "Texture.h"
#include "Material.h"

class BasicPhongMaterial : public Material {
private:
	Texture* colorTex;
	Texture* normalTex;
	Texture* specularTex;
	Texture* roughnessTex;
	Texture* emiTex;
	Texture* opacityTex;

	glm::vec3 baseColor;
	float specularity;
	float shininess;
	float opacity;
	float itileU;
	float itileV;
	bool receiveShadows;

public:

	BasicPhongMaterial(std::unordered_map<std::string, Shader*> shaders) : Material(shaders["BasicPhongShader"]), itileU(1.0), itileV(1.0), opacity(1.0), baseColor(glm::vec3(1.0f)), specularity(1), shininess(10),
		colorTex(nullptr), normalTex(nullptr), specularTex(nullptr), roughnessTex(nullptr), emiTex(nullptr), opacityTex(nullptr), receiveShadows(true){}

	BasicPhongMaterial(std::unordered_map<std::string, Shader*> shaders, MaterialParameters params) : Material(shaders["BasicPhongShader"],params), itileU(1.0), itileV(1.0), opacity(1.0), baseColor(glm::vec3(1.0f)), specularity(1), shininess(10),
		colorTex(nullptr), normalTex(nullptr), specularTex(nullptr), roughnessTex(nullptr), emiTex(nullptr), opacityTex(nullptr), receiveShadows(true) {}
	
	inline void setTileU(float u)	{	itileU = u;}
	inline float getTileU() { return itileU; }
	inline void setTileV(float v){itileV = v;}
	inline float getTileV() { return itileV; }
	inline void setBaseColor(glm::vec3 c) { baseColor = c; }
	inline glm::vec3 getBaseColor() { return baseColor; }
	inline void setSpecularity(float o) { specularity = o; }
	inline float getSpecularity() { return specularity; }
	inline void setShininess(float o) { shininess = o; }
	inline float getShininess() { return shininess; }
	inline void setOpacity(float o) { opacity = o; }
	inline float getOpacity() { return opacity; }
	inline void setReceiveShadows(bool c) { receiveShadows = c; }
	inline bool getReceiveShadows() { return receiveShadows; }

	void addColorTex(Texture* t);

	void addNormalTex(Texture* t);

	void addSpecularTex(Texture* t);

	void addRoughnessTex(Texture* t);

	void addOpacityTex(Texture* t);

	void bind(glm::mat4 proj, glm::mat4 view, glm::mat4 model);

	void unbind();

};
