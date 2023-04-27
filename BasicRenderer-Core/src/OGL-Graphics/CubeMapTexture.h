#pragma once
#include "Texture.h"
/// <summary>
/// Struct for setting each cubemap face texture path
/// </summary>
struct CubeMapFaces {
	std::string right;
	std::string left;
	std::string top;
	std::string bottom;
	std::string back;
	std::string front;
	CubeMapFaces(std::string r, std::string l, std::string t, std::string  b, std::string ba, std::string  f):
		right(r),left(l),top(t),bottom(b),back(ba),front(f){}
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

	CubeMapTexture(GLint level, GLint internalFormat, unsigned int w, unsigned int h, GLint border, GLenum format, GLenum type,
		 int magFilter, int minFilter, int wrapT, int wrapS, int wrapR);

	void bind(unsigned int slot) const;

	void unbind() const;

	virtual void generateTexture();



};

