#include "Framebuffer.h"

Framebuffer::Framebuffer(Texture* text,int attachmentType ,bool depthAttachment, bool resizeOnCallback) : m_TextureAttachment(text), resizeOnCallback(resizeOnCallback)
{
	width = text->getWidth();
	height = text->getHeight();
	m_MainAttachmentID = text->getID();
	m_DepthAttachmentID = -1;

	GLcall(glGenFramebuffers(1, &m_RendererID));
	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

	GLcall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, m_MainAttachmentID, 0));
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (depthAttachment) {

		GLcall(glGenRenderbuffers(1, &m_DepthAttachmentID));

		GLcall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachmentID));

		GLcall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));

		GLcall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachmentID));

	}

}

Framebuffer::Framebuffer(unsigned int w, unsigned int h, bool depthAttachment, bool resizeOnCallback) : width(w), height(h), resizeOnCallback(resizeOnCallback)
{



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

	if (m_DepthAttachmentID!=-1) {

		GLcall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachmentID));

		GLcall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h));

		GLcall(glBindRenderbuffer(GL_RENDERBUFFER, NULL));
	}
}
