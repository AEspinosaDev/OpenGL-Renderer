#pragma once
#include "Material.h"

/// <summary>
/// Epic's Unreal Engine 4 PBR Metallic-Roughness Workflow
/// </summary>
class PhysicalMaterial : public Material
{
	Texture*	m_AlbedoTex;
	Texture*	m_NormalTex;
	Texture*	m_RoughnessTex;
	Texture*	m_MetalnessTex;
	Texture*	m_EmissiveTex;
	Texture*	m_OpacityTex;
	Texture*	m_AOTex;

	//Overrides

	glm::vec3	m_Albedo;
	float		m_Roughness;
	float		m_Metalness;
	float		m_Emissive;
	float		m_Opacity;
	float		m_AO;

	float		m_TileU;
	float		m_TileV;

	bool	receiveShadows;

public:

	PhysicalMaterial() : Material("PhysicallyBasedShader"), m_TileU(1.0), m_TileV(1.0), m_Opacity(1.0), m_Albedo(glm::vec3(1.0f)), m_Roughness(0.5f), m_Metalness(0.0f), m_Emissive(0.0f), m_AO(1.0f),
		m_AlbedoTex(nullptr), m_NormalTex(nullptr), m_RoughnessTex(nullptr), m_MetalnessTex(nullptr), m_EmissiveTex(nullptr), m_OpacityTex(nullptr), m_AOTex(nullptr), receiveShadows(true) {}

	PhysicalMaterial(MaterialParameters params) : Material("PhysicallyBasedShader", params), m_TileU(1.0), m_TileV(1.0), m_Opacity(1.0), m_Albedo(glm::vec3(1.0f)), m_Roughness(0.5f), m_Metalness(0.0f), m_AO(1.0f), m_Emissive(0.0f), m_AlbedoTex(nullptr), m_NormalTex(nullptr), m_RoughnessTex(nullptr), m_MetalnessTex(nullptr), m_EmissiveTex(nullptr), m_OpacityTex(nullptr), m_AOTex(nullptr), receiveShadows(true) {}

#pragma region getters & setters

	inline void setTileU(float u) { m_TileU = u; }
	inline float getTileU() { return m_TileU; }

	inline void setTileV(float v) { m_TileV = v; }
	inline float getTileV() { return m_TileV; }

	inline void setAlbedoColor(glm::vec3 c) { m_Albedo = c; }
	inline glm::vec3 getAlbedoColor() { return m_Albedo; }

	inline void setRoughness(float o) { m_Roughness = o; }
	inline float getRoughness() { return m_Roughness; }

	inline void setMetalness(float o) { m_Metalness = o; }
	inline float getMetalness() { return m_Metalness; }

	inline void setEmissive(float o) { m_Emissive = o; }
	inline float getEmissive() { return m_Emissive; }

	inline void setOpacity(float o) { m_Opacity = o; }
	inline float getOpacity() { return m_Opacity; }

	inline void setAO(float o) { m_AO = o; }
	inline float getAO() { return m_AO; }

	inline void setReceiveShadows(bool c) { receiveShadows = c; }
	inline bool getReceiveShadows() { return receiveShadows; }

	inline void setAlbedoText(Texture* t) { m_AlbedoTex = t; }
	inline Texture* getAlbedoText() { return m_AlbedoTex; }

	inline void setNormalText(Texture* t) { m_NormalTex = t; }
	inline Texture* getNormalText() { return m_NormalTex; }

	inline void setRoughnessText(Texture* t) { m_RoughnessTex = t; }
	inline Texture* getRoughnessText() { return m_RoughnessTex; }

	inline void setMetalnessText(Texture* t) { m_MetalnessTex = t; }
	inline Texture* getMetalnessText() { return m_MetalnessTex; }

	inline void setEmissiveText(Texture* t) { m_EmissiveTex = t; }
	inline Texture* getEmissiveText() { return m_EmissiveTex; }

	inline void setAOText(Texture* t) { m_AOTex = t; }
	inline Texture* getAOText() { return m_AOTex; }

	inline void setOpacityMask(Texture* t) { m_OpacityTex = t; }
	Texture* getOpacityMask() { return m_OpacityTex; }

#pragma endregion

	void cacheUniforms();

	void decacheUniforms();

	void generateTextures();

};

