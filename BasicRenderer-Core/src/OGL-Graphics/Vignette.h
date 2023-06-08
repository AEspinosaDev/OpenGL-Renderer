#pragma once
#include <OGL-Graphics/VAO.h>
#include <OGL-Graphics/Texture.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


class Vignette
{
private:
	VAO* vao;
	Shader* shader;
	Texture* texture;

	const int vertexNum = 4;
	const int trisNum = 6;

public:
	Vignette(unsigned int w, unsigned int h);

	void draw();

	inline Shader* getShader() { return shader; }
	inline Texture* getTexture() { return texture; }
	inline Texture* setTexture(Texture* t) {
		Texture* returnText = texture; texture = t; return returnText;
}

};

