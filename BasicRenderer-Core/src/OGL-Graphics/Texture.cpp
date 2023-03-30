#include "Texture.h"


Texture::Texture(const std::string& path) :m_RendererID(0), m_FilePath(texturePath + path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_Samples(1),
level(0), internalFormat(GL_RGBA8), border(0), format(GL_RGBA), type(GL_UNSIGNED_BYTE) {

	generateTexture(true, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT);

}

Texture::Texture(unsigned int w, unsigned int h) : m_RendererID(0), m_FilePath("null"), m_LocalBuffer(nullptr), m_Width(w), m_Height(h), m_Samples(1),
level(0), internalFormat(GL_RGBA8), border(0), format(GL_RGBA), type(GL_UNSIGNED_BYTE) {

	generateTexture(false, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT);
	

}

Texture::Texture(unsigned int w, unsigned int h, unsigned int samples) : m_RendererID(0), m_FilePath("null"), m_LocalBuffer(nullptr), m_Width(w), m_Height(h), m_Samples(samples),
level(0), internalFormat(GL_RGBA8), border(0), format(GL_RGBA), type(GL_UNSIGNED_BYTE) {

	GLcall(glGenTextures(1, &m_RendererID));
	GLcall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID));
	GLcall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, m_Width, m_Height,
		GL_TRUE));
	GLcall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));

}

Texture::Texture(const std::string& path, GLint level, GLint internalFormat, unsigned int w, unsigned int h, GLint border, GLenum format, GLenum type,
	bool anisotropicFilter, int magFilter, int minFilter, int wrapT, int wrapS) :m_RendererID(0), m_FilePath(texturePath + path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_Samples(1),
	level(level), internalFormat(internalFormat), border(border), format(format), type(type) {

	generateTexture(anisotropicFilter, magFilter, minFilter, wrapT, wrapS);

}


Texture::Texture(GLint level, GLint internalFormat, unsigned int w, unsigned int h, GLint border, GLenum format, GLenum type, bool anisotropicFilter,
	int magFilter, int minFilter, int wrapT, int wrapS) :m_RendererID(0), m_FilePath("null"), m_LocalBuffer(nullptr), m_Width(w), m_Height(h), m_Samples(1),
	level(level), internalFormat(internalFormat), border(border), format(format), type(type) {

	generateTexture(anisotropicFilter, magFilter, minFilter, wrapT, wrapS);

}
void Texture::generateTexture(bool anisotropicFilter, int magFilter, int minFilter, int wrapT, int wrapS) {

	if (m_FilePath != "null") {
		m_LocalBuffer = loadTexture(m_FilePath.c_str(), m_Width, m_Height);
		/*int w, h, channels;
		m_LocalBuffer = stbi_load(m_FilePath.c_str(),&w, &h, &channels,0);
		m_Width = w;
		m_Height = h;*/
		if (!m_LocalBuffer)
		{
			std::cout << "Error cargando el fichero: "
				<< m_FilePath << std::endl;
			exit(-1);
		}
	}

	GLcall(glGenTextures(1, &m_RendererID));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	if (!m_LocalBuffer) {
		GLcall(glTexImage2D(GL_TEXTURE_2D, level, internalFormat, m_Width, m_Height, border, format,
			type,NULL));
	}
	else {
		GLcall(glTexImage2D(GL_TEXTURE_2D, level, internalFormat, m_Width, m_Height, border, format,
			type, m_LocalBuffer));
	}

	GLcall(glGenerateMipmap(GL_TEXTURE_2D));

	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		minFilter));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS));

	if (anisotropicFilter) {
		GLfloat fLargest;
		GLcall(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest));
		GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest));
	}
	GLcall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		delete[] m_LocalBuffer;
		//stbi_image_free(m_LocalBuffer);

}

void Texture::generateTexture(const std::string& path, GLint level, GLint internalFormat, unsigned int w, unsigned int h, GLint border, GLenum format, GLenum type, bool anisotropicFilter, int magFilter, int minFilter, int wrapT, int wrapS)
{
	m_FilePath = path;
	m_Width = w;
	m_Height = h;
	this->level = level;
	this->internalFormat = internalFormat;
	this->border = border;
	this->format = format;
	this->type = type;

	if (m_FilePath != "") {
		m_LocalBuffer = loadTexture(m_FilePath.c_str(), m_Width, m_Height);
		/*int w, h, channels;
		m_LocalBuffer = stbi_load(m_FilePath.c_str(), &w, &h, &channels, 0);
		m_Width = w;
		m_Height = h;*/
		if (!m_LocalBuffer)
		{
			std::cout << "Error cargando el fichero: "
				<< m_FilePath << std::endl;
			exit(-1);
		}
	}

	GLcall(glGenTextures(1, &m_RendererID));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	if (!m_LocalBuffer) {
		GLcall(glTexImage2D(GL_TEXTURE_2D, level, internalFormat, m_Width, m_Height, border, format,
			type, NULL));
	}
	else {
		GLcall(glTexImage2D(GL_TEXTURE_2D, level, internalFormat, m_Width, m_Height, border, format,
			type, m_LocalBuffer));
	}

	GLcall(glGenerateMipmap(GL_TEXTURE_2D));

	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		minFilter));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS));

	if (anisotropicFilter) {
		GLfloat fLargest;
		GLcall(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest));
		GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest));
	}
	GLcall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		delete[] m_LocalBuffer;
		//stbi_image_free(m_LocalBuffer);
}

Texture::~Texture() {
	GLcall(glDeleteTextures(1, &m_RendererID));
}

void Texture::bind(unsigned int slot) const {
	GLcall(glActiveTexture(GL_TEXTURE0 + slot));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_RendererID));


}
void Texture::unbind() const {
	GLcall(glBindTexture(GL_TEXTURE_2D, 0));
}



void Texture::resize(unsigned int w, unsigned int h)
{
	m_Width = w;
	m_Height = h;
	if (m_Samples == 1) {
		GLcall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
		GLcall(glTexImage2D(GL_TEXTURE_2D, level, internalFormat, m_Width, m_Height, border, format,
			type, NULL));
	}
	else {
		GLcall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID));
		GLcall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Samples, internalFormat, m_Width, m_Height,
			GL_TRUE));
	}

}

unsigned char* Texture::loadTexture(const char* fileName, unsigned int& w, unsigned int& h)
{
	FreeImage_Initialise(TRUE);

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);
	if (format == FIF_UNKNOWN)
		format = FreeImage_GetFIFFromFilename(fileName);
	if ((format == FIF_UNKNOWN) || !FreeImage_FIFSupportsReading(format))
		return NULL;

	FIBITMAP* img = FreeImage_Load(format, fileName);
	if (img == NULL)
		return NULL;

	FIBITMAP* tempImg = img;
	img = FreeImage_ConvertTo32Bits(img);
	FreeImage_Unload(tempImg);

	w = FreeImage_GetWidth(img);
	h = FreeImage_GetHeight(img);

	//BGRA a RGBA
	unsigned char* map = new unsigned char[4 * w * h];
	char* buff = (char*)FreeImage_GetBits(img);

	for (unsigned int j = 0; j < w * h; j++) {
		map[j * 4 + 0] = buff[j * 4 + 2];
		map[j * 4 + 1] = buff[j * 4 + 1];
		map[j * 4 + 2] = buff[j * 4 + 0];
		map[j * 4 + 3] = buff[j * 4 + 3];
	}

	FreeImage_Unload(img);
	FreeImage_DeInitialise();

	return map;
}