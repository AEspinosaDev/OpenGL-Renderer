#include "Framebuffer.h"

Framebuffer::Framebuffer(Texture* text, int attachmentType, bool depthAttachment, bool resizeOnCallback) : m_TextureAttachment(text), resizeOnCallback(resizeOnCallback),
attachmentType(attachmentType)
{
	width = text->getWidth();
	height = text->getHeight();
	m_MainAttachmentID = text->getID();
	m_DepthAttachmentID = -1;

	GLcall(glGenFramebuffers(1, &m_RendererID));
	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

	if (text->getSamples() == 1) {
		GLcall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, m_MainAttachmentID, 0));
	}
	else {
		GLcall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D_MULTISAMPLE, m_MainAttachmentID, 0));
	}

	if (depthAttachment) {

		GLcall(glGenRenderbuffers(1, &m_DepthAttachmentID));

		GLcall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachmentID));

		if (text->getSamples() == 1) {
			GLcall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
		}
		else {
			GLcall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, text->getSamples(), GL_DEPTH24_STENCIL8, width, height));
		}

		GLcall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachmentID));

	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, 0));


}

Framebuffer::Framebuffer(unsigned int w, unsigned int h, bool depthAttachment, bool resizeOnCallback) : width(w), height(h), resizeOnCallback(resizeOnCallback),
attachmentType(attachmentType)
{

}

void Framebuffer::setTextureAttachment(Texture* t)
{
	m_TextureAttachment = t;
	m_MainAttachmentID = t->getID();

	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

	if (t->getSamples() == 1) {
		GLcall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, m_MainAttachmentID, 0));
	}
	else {
		GLcall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D_MULTISAMPLE, m_MainAttachmentID, 0));
	}

	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::bind()
{
	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void Framebuffer::resize(unsigned int w, unsigned int h)
{
	if (!resizeOnCallback) return;

	width = w; height = h;

	m_TextureAttachment->resize(w, h);

	if (m_DepthAttachmentID != -1) {

		GLcall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachmentID));

		if (m_TextureAttachment->getSamples() == 1) {
			GLcall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h));
		}
		else {
			GLcall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_TextureAttachment->getSamples(), GL_DEPTH24_STENCIL8, w, h));
		}


		GLcall(glBindRenderbuffer(GL_RENDERBUFFER, NULL));
	}
}
