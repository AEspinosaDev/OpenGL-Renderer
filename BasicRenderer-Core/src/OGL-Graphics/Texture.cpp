#include "Texture.h"


Texture::Texture(const std::string& path) :m_RendererID(0), m_FilePath(texturePath + path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0),
	level(0), internalFormat(GL_RGBA8), border(0), format(GL_RGBA), type(GL_UNSIGNED_BYTE) {

	m_LocalBuffer = loadTexture(m_FilePath.c_str(), m_Width, m_Height);
	if (!m_LocalBuffer)
	{
		std::cout << "Error cargando el fichero: "
			<< path << std::endl;
		exit(-1);
	}

	GLcall(glGenTextures(1, &m_RendererID));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLcall(glTexImage2D(GL_TEXTURE_2D, level, internalFormat, m_Width, m_Height, border, format,
		type, (GLvoid*)m_LocalBuffer));

	GLcall(glGenerateMipmap(GL_TEXTURE_2D));

	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));

	//Filtro anisotropico
	/*if (glewIsExtensionSupported("GL_EXT_texture_filter_anisotropic")) {
		std::cout << "Filtro anisotropico soportado"<< std::endl;
	}*/

	GLfloat fLargest;
	GLcall(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest));

	GLcall(glBindTexture(GL_TEXTURE_2D, 0));


	delete[] m_LocalBuffer;

}

Texture::Texture(unsigned int w, unsigned int h) : m_RendererID(0), m_FilePath("null"), m_LocalBuffer(nullptr), m_Width(w), m_Height(h),
	level(0), internalFormat(GL_RGBA8), border(0), format(GL_RGBA), type(GL_UNSIGNED_BYTE) {

	GLcall(glGenTextures(1, &m_RendererID));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLcall(glTexImage2D(GL_TEXTURE_2D, level, internalFormat, m_Width, m_Height, border, format,
		type, NULL));

	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



	GLcall(glBindTexture(GL_TEXTURE_2D, 0));


}

Texture::Texture(const std::string& path, GLint level, GLint internalFormat, unsigned int w, unsigned int h, GLint border, GLenum format, GLenum type, 
	bool anisotropicFilter, int magFilter, int minFilter, int wrapT, int wrapS):m_RendererID(0), m_FilePath(texturePath + path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0),
	level(level), internalFormat(internalFormat), border(border), format(format), type(type) {


	m_LocalBuffer = loadTexture(m_FilePath.c_str(), m_Width, m_Height);
	if (!m_LocalBuffer)
	{
		std::cout << "Error cargando el fichero: "
			<< path << std::endl;
		exit(-1);
	}

	GLcall(glGenTextures(1, &m_RendererID));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLcall(glTexImage2D(GL_TEXTURE_2D, level, internalFormat, m_Width, m_Height, border, format,
		type, (GLvoid*)m_LocalBuffer));

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


	delete[] m_LocalBuffer;
}



Texture::Texture(GLint level, GLint internalFormat, unsigned int w, unsigned int h, GLint border, GLenum format, GLenum type, bool anisotropicFilter, 
	int magFilter, int minFilter, int wrapT, int wrapS) :m_RendererID(0), m_FilePath(""), m_LocalBuffer(nullptr), m_Width(w), m_Height(h),
	level(level), internalFormat(internalFormat), border(border), format(format), type(type) {

	GLcall(glGenTextures(1, &m_RendererID));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLcall(glTexImage2D(GL_TEXTURE_2D, level, internalFormat, m_Width, m_Height, border, format,
		type, NULL));


	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		minFilter));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT));
	GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS));
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	if (anisotropicFilter) {
		GLfloat fLargest;
		GLcall(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest));
		GLcall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest));
	}
	GLcall(glBindTexture(GL_TEXTURE_2D, 0));



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
	GLcall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLcall(glTexImage2D(GL_TEXTURE_2D, level, internalFormat, m_Width, m_Height, border, format,
		type, NULL));

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