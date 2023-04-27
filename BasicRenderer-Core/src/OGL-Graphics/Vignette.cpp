#include "Vignette.h"

Vignette::Vignette(unsigned int w, unsigned int h) : vao(new VAO()), shader(new Shader("VignetteShader.shader",ShaderType::UNLIT)), texture(new Texture(w, h))
{
	const float  quadTriangles[] = {
		// X, Y, Z,
		-1.0, -1.0, -5.0,// 0. left-bottom
		 1.0, -1.0, -5.0, // 1. right-bottom
		-1.0, 1.0, -5.0, // 2. left-top
		 1.0, 1.0, -5.0,  // 3. right-top
	};
	const float quadUVs[] = {
		// U, V
		0.0, 0.0, // 0. left-bottom
		 1.0, 0.0, // 1. right-bottom
		 0.0, 1.0, // 2. left-top
		 1.0, 1.0 // 3. right-top
	};

	const unsigned int ids[] = { 0,1,2,1,2,3 };

	texture->generateTexture();

	vao->bind();

	VBO pos_b(quadTriangles, vertexNum * sizeof(float) * 3);
	VBLayout layout0;
	layout0.push<float>(3);
	vao->addBuffer(pos_b, layout0);

	VBO normal_b(quadUVs, vertexNum * sizeof(float) * 2);
	VBLayout layout1;
	layout1.push<float>(2);
	vao->addBuffer(normal_b, layout1);

	IBO trisIndex(ids, trisNum);

	vao->unbind();

	shader->bind();
	shader->setInt("vignetteTex", 0);
	shader->unbind();
}

void Vignette::draw()
{
	glDisable(GL_CULL_FACE);

	shader->bind();


	shader->setMat4("u_proj", glm::ortho(-1, 1, -1, 1, 2, 250));

	texture->bind(0);

	vao->bind();

	GLcall(glDrawElements(GL_TRIANGLES, trisNum, GL_UNSIGNED_INT, (void*)0));

	shader->unbind();
}
