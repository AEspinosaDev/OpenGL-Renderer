#pragma once
#include "Texture.h"
#include "Material.h"

class UnlitBasicMaterial: public Material
{
private:
	Texture* colorTex;
	glm::vec3 color;
	float opacity;
	float itileU;
	float itileV;
public:
	UnlitBasicMaterial(std::unordered_map<std::string, Shader*> shaders) : Material(shaders["UnlitBasicShader"]), itileU(1.0), itileV(1.0),opacity(1.0), color(glm::vec3(1.0, 1.0, 1.0)),
		colorTex(nullptr) {}

	UnlitBasicMaterial(std::unordered_map<std::string, Shader*> shaders, MaterialParameters params) : Material(shaders["UnlitBasicShader"],params), itileU(1.0), itileV(1.0), opacity(1.0), color(glm::vec3(1.0, 1.0, 1.0)),
		colorTex(nullptr) {}

	inline void setTileU(float u) { itileU = u; }
	inline float getTileU() { return itileU; }
	inline void setTileV(float v) { itileV = v; }
	inline float getTileV() { return itileV; }
	inline void setOpacity(float o) { opacity = o; }
	inline float getOpacity() { return opacity; }


	inline void setColor(const glm::vec3  c) { color = c; }

	inline glm::vec3  getColor() { return color; }

	void addColorTex(Texture* t);

	void bind(glm::mat4 proj, glm::mat4 view, glm::mat4 model);

	void unbind();
};

