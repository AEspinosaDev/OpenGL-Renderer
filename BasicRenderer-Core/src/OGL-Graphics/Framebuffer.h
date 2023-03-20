#pragma once
#include <OGL-Graphics/Texture.h>

/// <summary>
/// Framebuffer class
/// </summary>
class Framebuffer
{
private:

	unsigned int m_RendererID;

	unsigned int m_DepthAttachmentID;

	unsigned int m_MainAttachmentID;

	Texture* m_TextureAttachment;

	unsigned int width;
	unsigned int height;
	bool resizeOnCallback;
public:

	/// <summary>
	/// Creates a framebuffer with a texture as color attachment.
	/// </summary>
	/// <param name="text">Texture</param>
	/// <param name="depthAttachment">Depth attachment enabling</param>
	/// <param name="resizeOnCallback">Resize enabling</param>
	Framebuffer(Texture* text, int attachmentType, bool depthAttachment, bool resizeOnCallback);

	/// <summary>
	///  Creates a framebuffer with a renderbuffer as color attachment.
	/// </summary>
	/// <param name="w">width</param>
	/// <param name="h">height</param>
	/// <param name="depthAttachment">Depth attachment enabling</param>
	/// <param name="resizeOnCallback">Resize enabling</param>
	Framebuffer(unsigned int w, unsigned int h , bool depthAttachment, bool resizeOnCallback);

	/// <summary>
	/// Creates a framebuffer with a texture as color attachment.
	/// </summary>
	/// <param name="text">Texture</param>
	/// <param name="depthAttachment">Depth attachment enabling</param>
	/// <param name="resizeOnCallback">Resize enabling</param>
	Framebuffer(Texture* text, bool resizeOnCallback);

	inline void setWidth(unsigned int w) { width = w; }
	inline void setHeight(unsigned int h) { height = h; }
	inline unsigned int getWidth() { return width; }
	inline unsigned int getHeight() { return height; }
	inline Texture* getTextureAttachment() { return m_TextureAttachment; }

	/// <summary>
	/// Tell OpenGL to activate this framebuffer
	/// </summary>
	void bind();

	/// <summary>
	/// Resize framebuffer
	/// </summary>
	/// <param name="w">New width</param>
	/// <param name="h">New height</param>
	void resize(unsigned int w, unsigned int h);

	/// <summary>
	/// Load renderbuffer data to color texture
	/// </summary>
	void blit();



};

