#pragma once
#include <vector>
#include "Renderer.h"

/// <summary>
/// Class that manages all lights instances and data.
/// </summary>
class LightManager
{

public:
	static const int MAX_LIGHTS = 32;
	static float shadowFarPlane;
	static void generateShadowMaps();
	static void uploadLightDataToShader(Shader* s);
	static void renderShadows();
};
