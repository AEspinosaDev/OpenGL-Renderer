#include "Texture.h"


Texture::Texture(const std::string& path) :m_RendererID(0), m_FilePath(texturePath + path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_Samples(1), m_TextureType(TextureType::TEXTURE_2D), isDirty(true) {

	m_TextConfig.anisotropicFilter = true;
	m_TextConfig.level = 0;
	m_TextConfig.internalFormat = GL_RGBA8;
	m_TextConfig.border = 0;
	m_TextConfig.format = GL_RGBA;
	m_TextConfig.type = GL_UNSIGNED_BYTE;
	m_TextConfig.magFilter = GL_LINEAR;
	m_TextConfig.minFilter = GL_LINEAR_MIPMAP_LINEAR;
	m_TextConfig.wrapS = GL_REPEAT;
	m_TextConfig.wrapT = GL_REPEAT;
	m_TextConfig.wrapR = -1;

	cacheTextureImage();
}

Texture::Texture(unsigned int w, unsigned int h) : m_RendererID(0), m_FilePath("null"), m_LocalBuffer(nullptr), m_Width(w), m_Height(h), m_Samples(1), m_TextureType(TextureType::TEXTURE_2D), isDirty(true) {

	m_TextConfig.anisotropicFilter = false;
	m_TextConfig.level = 0;
	m_TextConfig.internalFormat = GL_RGBA8;
	m_TextConfig.border = 0;
	m_TextConfig.format = GL_RGBA;
	m_TextConfig.type = GL_UNSIGNED_BYTE;
	m_TextConfig.magFilter = GL_NEAREST;
	m_TextConfig.minFilter = GL_NEAREST;
	m_TextConfig.wrapS = GL_REPEAT;
	m_TextConfig.wrapT = GL_REPEAT;
	m_TextConfig.wrapR = -1;

	

}

Texture::Texture(unsigned int w, unsigned int h, unsigned int samples) : m_RendererID(0), m_FilePath("null"), m_LocalBuffer(nullptr), m_Width(w), m_Height(h), m_Samples(samples),
 m_TextureType(TextureType::TEXTURE_2D), isDirty(true) {

	m_TextConfig.anisotropicFilter = false;
	m_TextConfig.level = 0;
	m_TextConfig.internalFormat = GL_RGBA8;
	m_TextConfig.border = 0;
	m_TextConfig.format = GL_RGBA;
	m_TextConfig.type = GL_UNSIGNED_BYTE;
	m_TextConfig.magFilter = GL_NEAREST;
	m_TextConfig.minFilter = GL_NEAREST;
	m_TextConfig.wrapS = GL_REPEAT;
	m_TextConfig.wrapT = GL_REPEAT;
	m_TextConfig.wrapR = -1;

	GLcall(glGenTextures(1, &m_RendererID));
	GLcall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID));
	GLcall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, m_TextConfig.internalFormat, m_Width, m_Height,
		GL_TRUE));
	GLcall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));

}

Texture::Texture(const std::string& path, GLint level, GLint internalFormat, unsigned int w, unsigned int h, GLint border, GLenum format, GLenum type,
	bool anisotropicFilter, int magFilter, int minFilter, int wrapT, int wrapS) :m_RendererID(0), m_FilePath(texturePath + path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_Samples(1), m_TextureType(TextureType::TEXTURE_2D), isDirty(true) {

	m_TextConfig.anisotropicFilter = anisotropicFilter;
	m_TextConfig.level = level;
	m_TextConfig.internalFormat = internalFormat;
	m_TextConfig.border = border;
	m_TextConfig.format = format;
	m_TextConfig.type = type;
	m_TextConfig.magFilter = magFilter;
	m_TextConfig.minFilter = minFilter;
	m_TextConfig.wrapS = wrapS;
	m_TextConfig.wrapT = wrapT;
	m_TextConfig.wrapR = wrapT;
	
	cacheTextureImage();
}


Texture::Texture(GLint level, GLint internalFormat, unsigned int w, unsigned int h, GLint border, GLenum format, GLenum type, bool anisotropicFilter,
	int magFilter, int minFilter, int wrapT, int wrapS) :m_RendererID(0), m_FilePath("null"), m_LocalBuffer(nullptr), m_Width(w), m_Height(h), m_Samples(1),
	m_TextureType(TextureType::TEXTURE_2D), isDirty(true) {

	m_TextConfig.anisotropicFilter = anisotropicFilter;
	m_TextConfig.level = level;
	m_TextConfig.internalFormat = internalFormat;
	m_TextConfig.border = border;
	m_TextConfig.format = format;
	m_TextConfig.type = type;
	m_TextConfig.magFilter = magFilter;
	m_TextConfig.minFilter = minFilter;
	m_TextConfig.wrapS = wrapS;
	m_TextConfig.wrapT = wrapT;
	m_TextConfig.wrapR = wrapT;

	
}

void Texture::generateTexture() {
	if (!isDirty) return;

	GLcall(glGenTextures(1, &m_RendererID));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	if (!m_LocalBuffer) {
		GLcall(glTexImage2D(
			GL_TEXTURE_2D,
			m_TextConfig.level,
			m_TextConfig.internalFormat,
			m_Width,
			m_Height,
			m_TextConfig.border,
			m_TextConfig.format,
			m_TextConfig.type,
			NULL
		));
	}
	else {
		GLcall(glTexImage2D(
			GL_TEXTURE_2D,
			m_TextConfig.level,
			m_TextConfig.internalFormat,
			m_Width,
			m_Height,
			m_TextConfig.border,
			m_TextConfig.format,
			m_TextConfig.type,
			m_LocalBuffer
		));
	}

	GLcall(glGenerateMipmap(GL_TEXTURE_2D));

	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,	m_TextConfig.minFilter));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,	m_TextConfig.magFilter));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,		m_TextConfig.wrapT));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,		m_TextConfig.wrapS));

	if (m_TextConfig.anisotropicFilter) {

		GLfloat fLargest;
		GLcall(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest));
		GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest));

	}
	GLcall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		delete[] m_LocalBuffer;
	//stbi_image_free(m_LocalBuffer);

	isDirty = false;

	std::cout << "Texture loaded" << std::endl;


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
		GLcall(glTexImage2D(GL_TEXTURE_2D, m_TextConfig.level, m_TextConfig.internalFormat, m_Width, m_Height, m_TextConfig.border, m_TextConfig.format,
			m_TextConfig.type, NULL));
	}
	else {
		GLcall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID));
		GLcall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Samples, m_TextConfig.internalFormat, m_Width, m_Height,
			GL_TRUE));
	}

}

void Texture::cacheTextureImage()
{
		m_LocalBuffer = loadTexture(m_FilePath.c_str(), m_Width, m_Height);

		if (!m_LocalBuffer)
		{
			std::cout << "Error cargando el fichero: "
				<< m_FilePath << std::endl;
			exit(-1);
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