#pragma once
#include "Material.h"
#include <OGL-Graphics/CubeMapTexture.h>

class SkyboxMaterial : public Material
{
	CubeMapTexture* cubeMapText;

public:
	SkyboxMaterial() : Material("SkyboxShader"), cubeMapText(nullptr)
	{}

	SkyboxMaterial(CubeMapTexture* cubemap) : Material("SkyboxShader"), cubeMapText(cubemap)
	{}

	inline CubeMapTexture* getTexture() { return cubeMapText; }
	inline void setTexture(CubeMapTexture* cubemap) { /*delete[] cubeMapText;*/ cubeMapText = cubemap; }

	void cacheUniforms();

	void decacheUniforms();

	void generateTextures();


};

