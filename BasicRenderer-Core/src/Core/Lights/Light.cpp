#include "Light.h"


void Light::setShadowText(Texture* t)
{
	t->generateTexture();
	m_ShadowText = t;
}

