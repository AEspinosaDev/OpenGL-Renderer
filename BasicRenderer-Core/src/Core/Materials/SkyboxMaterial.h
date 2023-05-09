#pragma once
#include "Material.h"
#include <OGL-Graphics/CubeMapTexture.h>

class SkyboxMaterial : public Material
{
	CubeMapTexture* m_CubeMapText;

public:
	SkyboxMaterial() : Material("SkyboxShader"), m_CubeMapText(nullptr)
	{}

	SkyboxMaterial(CubeMapTexture* cubemap) : Material("SkyboxShader"), m_CubeMapText(cubemap)
	{}

	inline CubeMapTexture* getTexture() { return m_CubeMapText; }
	inline void setTexture(CubeMapTexture* cubemap) { /*delete[] cubeMapText;*/ m_CubeMapText = cubemap; }

	void cacheUniforms();

	void decacheUniforms();

	void generateTextures();


};

