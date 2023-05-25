#pragma once
#include <ext/vector_float4.hpp>
#include <string>

enum AntialiasingType {
	NONE = 0,
	MSAAx2 = 2,
	MSAAx4 = 4,
	MSAAx8 = 8,
	MSAAx16 = 16
};
enum ShadowMapQuality {
	VERY_LOW = 512,
	LOW = 1080,
	MID = 2048,
	HIGH = 4096,
	VERY_HIGHT = 8192
};

struct PossProcessEffects {
	bool												gammaCorrection;
	bool												bloom;
};

struct UtilityParameters {
	float												secondCounter;
	unsigned int										fps;
	float												mouselastX;
	float												mouselastY;
	bool												firstMouse;
	float												deltaTime;
	float												lastFrame;
	unsigned int										lastWidth;
	unsigned int										lastHeight;

	UtilityParameters() {
		secondCounter = 0;
		fps = 0;
		firstMouse = true;
		deltaTime = 0.0;
		lastFrame = 0.0;
		mouselastX = 0;
		mouselastY = 0;
		lastWidth = 0;
		lastHeight = 0;
	}
};


struct UISettings {


};

struct GlobalSettings {
	bool												isFullscreen;
	bool												vsync;
	unsigned int										shadowResolution;
	float												shadowFarPlane;
	unsigned int										antialiasingSamples;
	bool												postProcess;
	PossProcessEffects									ppEffects;
	bool												editMode;
	glm::vec4											clearColor;
	//UISettings											UI_Settings;

	GlobalSettings() {
		isFullscreen = false;
		vsync = true;
		shadowResolution = ShadowMapQuality::MID;
		shadowFarPlane = 100.0;
		antialiasingSamples = AntialiasingType::MSAAx16;
		postProcess = false;
		ppEffects.bloom = false;
		ppEffects.gammaCorrection = true;
		editMode = true;
		clearColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
		//UI_Settings;
	}
};


