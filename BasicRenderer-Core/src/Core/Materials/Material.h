#pragma once
#include <Shader.h>

enum BlendingType {
	NORMAL,
	ADDITIVE,
	CUSTOM
};
enum FaceVisibility {
	FRONT,
	BACK,
	BOTH
};

/// <summary>
/// Parameters needed for material to render properly
/// </summary>
struct MaterialParameters {
	bool alphaTest = false;
	bool transparency = false;
	BlendingType blendingType = BlendingType::NORMAL;
	FaceVisibility faceVisibility= FaceVisibility::FRONT;
	//transparency funtions, src alphas and target
};


/// <summary>
/// Basic pure abstract class for material
/// </summary>
class Material {
protected:
	const std::string m_ShaderName_ID;
	Shader* m_shader;
	MaterialParameters m_Parameters;

	bool isDirty;
	
public:

	Material(std::string shaderName) : m_shader(nullptr) , m_ShaderName_ID(shaderName) , isDirty(true) {}

	Material(std::string shaderName, MaterialParameters params) : m_shader(nullptr),m_Parameters(params) , m_ShaderName_ID(shaderName), isDirty(true) {}

	virtual inline const std::string getShaderNameID() { return m_ShaderName_ID; }
	
	virtual inline void setShader(Shader* s) { m_shader = s; }

	virtual inline Shader* getShader() { return m_shader; }

	virtual inline MaterialParameters getParameters() { return m_Parameters; }

	virtual inline void setParameters(MaterialParameters params) { m_Parameters = params; }

	virtual inline bool getTransparency() { return m_Parameters.transparency; }

	//virtual Texture* getMask() { return nullptr; };

	virtual inline void setTransparency(bool t) { m_Parameters.transparency = t; }

	virtual inline BlendingType getBlending() { return m_Parameters.blendingType; }

	virtual inline void setBlending(BlendingType t) { m_Parameters.blendingType = t; }

	virtual inline FaceVisibility getFaceVisibility() { return m_Parameters.faceVisibility; }

	virtual inline void setFaceVisibility(FaceVisibility v) { m_Parameters.faceVisibility = v; }
	
	virtual void cacheUniforms() = 0;

	virtual void decacheUniforms() = 0;

	virtual void setupParameters();

	virtual void generateTextures() = 0;
};

