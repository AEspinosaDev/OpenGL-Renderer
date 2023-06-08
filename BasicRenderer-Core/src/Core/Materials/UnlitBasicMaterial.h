#pragma once
#include "Texture.h"
#include "Material.h"

class UnlitBasicMaterial: public Material
{
private:
	Texture*	m_ColorText;
	Texture*	m_OpacityText;

	glm::vec3	m_Color;
	bool		m_OverrideColor;
	float       m_OverrideColorStr;
	float		m_Opacity;

	float		m_TileU;
	float		m_TileV;

public:
	UnlitBasicMaterial() : Material("UnlitBasicShader"), m_TileU(1.0), m_TileV(1.0),m_Opacity(1.0), m_Color(glm::vec3(1.0, 1.0, 1.0)), m_OverrideColor(false), m_OverrideColorStr(0.5),
		m_ColorText(nullptr), m_OpacityText(nullptr) {}

	UnlitBasicMaterial(MaterialParameters params) : Material("UnlitBasicShader",params), m_TileU(1.0), m_TileV(1.0), m_Opacity(1.0), m_Color(glm::vec3(1.0, 1.0, 1.0)), m_OverrideColor(false), m_OverrideColorStr(0.5),
		m_ColorText(nullptr),m_OpacityText(nullptr) {}

	inline void setTileU(float u) { m_TileU = u; }
	inline float getTileU() { return m_TileU; }
	inline void setTileV(float v) { m_TileV = v; }
	inline float getTileV() { return m_TileV; }

	inline void setOpacity(float o) { m_Opacity = o; }
	inline float getOpacity() { return m_Opacity; }

	virtual Texture* getOpacityMask() { return m_OpacityText; }

	inline void setColor(const glm::vec3  c) { m_Color = c; }
	inline glm::vec3  getColor() { return m_Color; }
	inline void setOverrideColor(bool op) { m_OverrideColor = op; }
	inline bool getOverrideColor() { return m_OverrideColor; }
	inline void setOverrideColorStrenth(float op) { m_OverrideColorStr = op; }
	inline float getOverrideColorStrenght() { return m_OverrideColorStr; }

	inline void addOpacityTex(Texture* t) { m_OpacityText = t; }

	inline void addColorTex(Texture* t) { m_ColorText = t; }

	void cacheUniforms();

	void decacheUniforms();

	void generateTextures();
};

