#pragma once
#include "Texture.h"
#include "Material.h"

class BasicPhongMaterial : public Material {
private:
	Texture*	m_ColorTex;
	Texture*	m_NormalTex;
	Texture*	m_SpecularTex;
	Texture*	m_ShininessTex;
	Texture*	m_EmiTex;
	Texture*	m_OpacityTex;

	glm::vec3	m_BaseColor;
	float		m_Specularity;
	float		m_Shininess;
	float		m_Opacity;

	float		m_TileU;
	float		m_TileV;

	bool		receiveShadows;

public:

	BasicPhongMaterial() : Material("BasicPhongShader"), m_TileU(1.0), m_TileV(1.0), m_Opacity(1.0), m_BaseColor(glm::vec3(1.0f)), m_Specularity(1), m_Shininess(10),
		m_ColorTex(nullptr), m_NormalTex(nullptr), m_SpecularTex(nullptr), m_ShininessTex(nullptr), m_EmiTex(nullptr), m_OpacityTex(nullptr), receiveShadows(true){}

	BasicPhongMaterial(MaterialParameters params) : Material("BasicPhongShader",params), m_TileU(1.0), m_TileV(1.0), m_Opacity(1.0), m_BaseColor(glm::vec3(1.0f)), m_Specularity(1), m_Shininess(10),
		m_ColorTex(nullptr), m_NormalTex(nullptr), m_SpecularTex(nullptr), m_ShininessTex(nullptr), m_EmiTex(nullptr), m_OpacityTex(nullptr), receiveShadows(true) {}
	

#pragma region getters & setters

	inline void setTileU(float u)	{	m_TileU = u;}
	inline float getTileU() { return m_TileU; }

	inline void setTileV(float v){m_TileV = v;}
	inline float getTileV() { return m_TileV; }

	inline void setBaseColor(glm::vec3 c) { m_BaseColor = c; }
	inline glm::vec3 getBaseColor() { return m_BaseColor; }

	inline void setSpecularity(float o) { m_Specularity = o; }
	inline float getSpecularity() { return m_Specularity; }

	inline void setShininess(float o) { m_Shininess = o; }
	inline float getShininess() { return m_Shininess; }

	inline void setOpacity(float o) { m_Opacity = o; }
	inline float getOpacity() { return m_Opacity; }

	inline void setReceiveShadows(bool c) { receiveShadows = c; }
	inline bool getReceiveShadows() { return receiveShadows; }

	inline void addColorTex(Texture* t) { m_ColorTex = t; }

	inline void addNormalTex(Texture* t) { m_NormalTex = t; }

	inline void addSpecularTex(Texture* t) { m_SpecularTex = t; }

	inline void addShininessTex(Texture* t) { m_ShininessTex = t; }

	inline void addOpacityTex(Texture* t) { m_OpacityTex = t; }

	Texture* getOpacityMask() { return m_OpacityTex; }

#pragma endregion

	void cacheUniforms();

	void decacheUniforms();

	void generateTextures();

};
