#include "Framebuffer.h"

Framebuffer::Framebuffer(Texture* text, int attachmentType, GLenum textarget, bool depthAttachment, bool resizeOnCallback) : m_TextureAttachment(text), resizeOnCallback(resizeOnCallback),
attachmentType(attachmentType)
{
	width = text->getWidth();
	height = text->getHeight();
	m_MainAttachmentID = text->getID();
	m_DepthAttachmentID = -1;

	GLcall(glGenFramebuffers(1, &m_RendererID));
	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

	switch (text->getType())
	{
	case TEXTURE_2D:
		if (text->getSamples() == 1) {
			GLcall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textarget, m_MainAttachmentID, 0));

		}
		else {
			GLcall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D_MULTISAMPLE, m_MainAttachmentID, 0));
		}

		break;
	case TEXTURE_CUBE:
		GLcall(glFramebufferTexture(GL_FRAMEBUFFER, attachmentType, m_MainAttachmentID, 0));
		break;
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
		std::cout << "ERROR::FRAMEBUFFER::" << m_RendererID << ":: Framebuffer is not complete!" << std::endl;

	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, 0));


}

Framebuffer::Framebuffer(unsigned int w, unsigned int h, bool depthAttachment, bool resizeOnCallback) : width(w), height(h), resizeOnCallback(resizeOnCallback),
attachmentType(attachmentType)
{

}

void Framebuffer::setTextureAttachment(Texture* t, GLenum textarget)
{
	m_TextureAttachment = t;
	m_MainAttachmentID = t->getID();

	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

	switch (t->getType())
	{
	case TEXTURE_2D:
		if (t->getSamples() == 1) {
			GLcall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textarget, m_MainAttachmentID, 0));

		}
		else {
			GLcall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D_MULTISAMPLE, m_MainAttachmentID, 0));
		}

		break;
	case TEXTURE_CUBE:
		GLcall(glFramebufferTexture(GL_FRAMEBUFFER, attachmentType, m_MainAttachmentID, 0));
		break;
	}


	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::setTextureAttachmentSamples(AntialiasingType samples)
{
	if (samples == 0) return;
	m_TextureAttachment->changeSampleNumber(samples);
	GLcall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachmentID));
	GLcall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_TextureAttachment->getSamples(), GL_DEPTH24_STENCIL8, width, height));
	GLcall(glBindRenderbuffer(GL_RENDERBUFFER, NULL));
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
