#pragma once
//#include "Renderer.h"
#include "VAO.h"
#include "IBO.h"
#include "Shader.h"
#include "FreeImage.h"


class Texture {

	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	unsigned int m_Width, m_Height;
	unsigned int m_Samples;

	GLint level;
	GLint internalFormat;
	GLint border;
	GLenum format;
	GLenum type;


public:
	/// <summary>
	/// Creates a standard tileable anisotropic filtered color texture given an image file.
	/// </summary>
	/// <param name="path">File name</param>
	Texture(const std::string& path);

	/// <summary>
	/// Creates an standard tileable empty texture given its dimensions.
	/// </summary>
	/// <param name="w">Width</param>
	/// <param name="h">Height</param>
	Texture(unsigned int w, unsigned int h);

	/// <summary>
	/// Creates a multisampled tileable empty texture given its dimensions.
	/// </summary>
	/// <param name="w">Width</param>
	/// <param name="h">Height</param>
	/// <param name="samples">Number of samples</param>
	Texture(unsigned int w, unsigned int h, unsigned int samples);

	/// <summary>
	/// Creates a complete custom texture given all necessary parameters and image file
	/// </summary>
	/// <param name="path">File name</param>
	/// <param name="level">Mipmap level</param>
	/// <param name="internalFormat">GL Internal format (eg: GL_RGBA8)</param>
	/// <param name="w">Width</param>
	/// <param name="h">Height</param>
	/// <param name="border">Border</param>
	/// <param name="format">GL Format (eg: GL_RGBA)</param>
	/// <param name="type">Type of data</param>
	/// <param name="anisotropicFilter">Enable anisotropic filter</param>
	/// <param name="magFilter">GL_TEXTURE_MAG_FILTER</param>
	/// <param name="minFilter">GL_TEXTURE_MIN_FILTER</param>
	/// <param name="wrapT">GL_TEXTURE_WRAP_T</param>
	/// <param name="wrapS">GL_TEXTURE_WRAP_S</param>
	Texture(const std::string& path, GLint level, GLint internalFormat ,unsigned int w, unsigned int h, GLint border, GLenum format, GLenum type,
		bool anisotropicFilter,int magFilter, int minFilter, int wrapT, int wrapS);

	/// <summary>
	/// Creates a complete multisampled custom texture given all necessary parameters and image file
	/// </summary>
	/// <param name="path">File name</param>
	/// <param name="level">Mipmap level</param>
	/// <param name="internalFormat">GL Internal format (eg: GL_RGBA8)</param>
	/// <param name="w">Width</param>
	/// <param name="h">Height</param>
	/// <param name="samples">Number of samples</param>
	/// <param name="border">Border</param>
	/// <param name="format">GL Format (eg: GL_RGBA)</param>
	/// <param name="type">Type of data</param>
	/// <param name="anisotropicFilter">Enable anisotropic filter</param>
	/// <param name="magFilter">GL_TEXTURE_MAG_FILTER</param>
	/// <param name="minFilter">GL_TEXTURE_MIN_FILTER</param>
	/// <param name="wrapT">GL_TEXTURE_WRAP_T</param>
	/// <param name="wrapS">GL_TEXTURE_WRAP_S</param>
	/*Texture(const std::string& path, GLint level, GLint internalFormat, unsigned int w, unsigned int h, unsigned int samples, GLint border, GLenum format, GLenum type,
		bool anisotropicFilter, int magFilter, int minFilter, int wrapT, int wrapS);*/

	/// <summary>
	/// Creates a complete custom texture given all necessary parameters and image file
	/// </summary>
	/// <param name="level">Mipmap level</param>
	/// <param name="internalFormat">GL Internal format (eg: GL_RGBA8)</param>
	/// <param name="w">Width</param>
	/// <param name="h">Height</param>
	/// <param name="border">Border</param>
	/// <param name="format">GL Format (eg: GL_RGBA)</param>
	/// <param name="type">Type of data</param>
	/// <param name="anisotropicFilter">Enable anisotropic filter</param>
	/// <param name="magFilter">GL_TEXTURE_MAG_FILTER</param>
	/// <param name="minFilter">GL_TEXTURE_MIN_FILTER</param>
	/// <param name="wrapT">GL_TEXTURE_WRAP_T</param>
	/// <param name="wrapS">GL_TEXTURE_WRAP_S</param>
	Texture(GLint level, GLint internalFormat ,unsigned int w, unsigned int h, GLint border, GLenum format, GLenum type,
		bool anisotropicFilter, int magFilter, int minFilter, int wrapT, int wrapS);

	~Texture();

	/// <summary>
	/// Activates the texture
	/// </summary>
	/// <param name="slot">GL memory slot</param>
	void bind(unsigned int slot) const;

	/// <summary>
	/// Deactivates the texture
	/// </summary>
	void unbind() const;

	inline unsigned int getID() const { return m_RendererID; }
	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }
	inline int getSamples() const { return m_Samples; }

	void resize(unsigned int w, unsigned int h);


private:
	unsigned char* loadTexture(const char* fileName, unsigned int& w, unsigned int& h);
	
};
