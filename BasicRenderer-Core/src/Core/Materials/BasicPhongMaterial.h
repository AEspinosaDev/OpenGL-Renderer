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

	float opacity;
	float itileU;
	float itileV;

public:

	BasicPhongMaterial(std::unordered_map<std::string, Shader*> shaders) : Material(shaders["BasicPhongShader"]), itileU(1.0), itileV(1.0), opacity(1.0),
		colorTex(nullptr), normalTex(nullptr), specularTex(nullptr), roughnessTex(nullptr), emiTex(nullptr), opacityTex(nullptr) {}

	BasicPhongMaterial(std::unordered_map<std::string, Shader*> shaders, MaterialParameters params) : Material(shaders["BasicPhongShader"],params), itileU(1.0), itileV(1.0), opacity(1.0),
		colorTex(nullptr), normalTex(nullptr), specularTex(nullptr), roughnessTex(nullptr), emiTex(nullptr), opacityTex(nullptr) {}
	
	inline void setTileU(float u)	{	itileU = u;}
	inline float getTileU() { return itileU; }
	inline void setTileV(float v){itileV = v;}
	inline float getTileV() { return itileV; }
	inline void setOpacity(float o) { opacity = o; }
	inline float getOpacity() { return opacity; }

	void addColorTex(Texture* t);

	void addNormalTex(Texture* t);

	void addSpecularTex(Texture* t);

	void addRoughnessTex(Texture* t);

	void addOpacityTex(Texture* t);

	void bind(glm::mat4 proj, glm::mat4 view, glm::mat4 model);

	void unbind();

};
