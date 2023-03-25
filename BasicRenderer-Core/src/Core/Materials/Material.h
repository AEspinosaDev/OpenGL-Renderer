#pragma once
#include <Shader.h>

/// <summary>
/// Basic pure abstract class for material
/// </summary>
class Material {
protected:
	Shader* m_shader;
	//Transparency
	//transparency funtions, src alphas and target
	//cull facing double, back or front
	//

public:

	Material(Shader* shader): m_shader(shader){}
	
	virtual inline Shader* getShader() { return m_shader; }
	
	virtual void bind(glm::mat4 proj, glm::mat4 view, glm::mat4 model) = 0;

	virtual void unbind() = 0;
};

