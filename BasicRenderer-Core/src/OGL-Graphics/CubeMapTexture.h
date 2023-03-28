#pragma once
#include "Texture.h"
/// <summary>
/// Struct for setting each cubemap face texture path
/// </summary>
struct CubeMapFaces {
	std::string& right;
	std::string& left;
	std::string& top;
	std::string& bottom;
	std::string& back;
	std::string& front;
};

/// <summary>
/// Cubemap texture class
/// </summary>
class CubeMapTexture: public Texture
{
	const int NUM_FACES = 6;
	CubeMapFaces m_FacesPath;



public:
	CubeMapTexture(CubeMapFaces paths);


	void bind(unsigned int slot) const;

	void unbind() const;

private:
	void generateCubeMapTexture(int magFilter, int minFilter, int wrapT, int wrapS, int wrapR);
};

