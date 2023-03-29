#pragma once
#include "Material.h"
#include <OGL-Graphics/CubeMapTexture.h>

class SkyboxMaterial : public Material
{
	CubeMapTexture* cubeMapText;

public:
	SkyboxMaterial(std::unordered_map<std::string, Shader*> shaders) : Material(shaders["SkyboxShader"]), cubeMapText(nullptr)
	{}

	SkyboxMaterial(CubeMapTexture* cubemap ,std::unordered_map<std::string, Shader*> shaders) : Material(shaders["SkyboxShader"]), cubeMapText(cubemap)
	{}

	inline CubeMapTexture* getTexture() { return cubeMapText; }
	inline void setTexture(CubeMapTexture* cubemap) { /*delete[] cubeMapText;*/ cubeMapText = cubemap; }

	void bind(glm::mat4 proj, glm::mat4 view, glm::mat4 model);

	void unbind();

};

