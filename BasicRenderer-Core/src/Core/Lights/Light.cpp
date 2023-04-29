#include "Light.h"


void Light::setShadowText(Texture* t)
{
	t->generateTexture();
	m_ShadowText = t;
}

void Light::draw(glm::mat4 proj, glm::mat4 view)
{
	//setTransform();
}