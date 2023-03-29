#pragma once
#include <Core/Materials/SkyboxMaterial.h>
#include <OGL-Graphics/VAO.h>

class SkyboxMesh
{
	VAO m_Vao;
	SkyboxMaterial* m_Mat;
	const rsize_t VERTEX_NUM = 36;

public:
	SkyboxMesh(SkyboxMaterial* m);

	inline void setMaterial(SkyboxMaterial* material) {
		m_Mat = material;
	}

	inline SkyboxMaterial* getMaterial() {
		return m_Mat;
	}

	void draw(glm::mat4 proj, glm::mat4 view); 



	


};

