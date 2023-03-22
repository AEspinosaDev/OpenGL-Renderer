#pragma once
#include <Shader.h>
#include <Texture.h>

class Material {
protected:

	Shader* m_shader;
	Texture* colorTex;
	Texture* normalTex;
	Texture* specularTex;
	Texture* roughnessTex;
	Texture* emiTex;

	float itileU;
	float itileV;

public:
	Material(Shader* shader, const std::string& colorPath, const std::string& normalPath, const std::string& specularPath, const std::string& roughnessPath, const std::string& emiPath) ;

	Material(Shader* shader);
	
	~Material(){}

	void bindTextures();


	inline Shader* getShader() { return m_shader; }

	void addColorTex(Texture* t);

	void addNormalTex(Texture* t);

	void addSpecularTex(Texture* t);

	void addRoughnessTex(Texture* t);

	inline void setTileU(float u)
	{
		itileU = u;
	}

	inline float getTileU() { return itileU; }

	inline void SetTileV(float v)
	{
		itileV = v;
	}

	inline float getTileV() { return itileV; }

};

