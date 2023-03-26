#include "Material.h"

void Material::setupParameters()
{
	//FACE VISIBILITY
	switch (m_Parameters.faceVisibility)
	{
	case 0:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		break;
	case 1:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		break;
	case 2:
		glDisable(GL_CULL_FACE);
		break;
	}
	//BLENDING
	if (m_Parameters.transparency) {
		glEnable(GL_BLEND);
		switch (m_Parameters.blendingType)
		{
		case 0:
			GLcall(glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO));
			break;
		case 1:
			GLcall(glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO));
			break;
		case 2:
			GLcall(glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO));
			break;
		}
	}
	else {
		glDisable(GL_BLEND);
	}

}
