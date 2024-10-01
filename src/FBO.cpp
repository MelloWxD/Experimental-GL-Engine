#include "..\Header\FBO.h"
#include "..\Header\Texture.h"

FBO::FBO(unsigned t = FBO_DEFAULT)
{
	if (t == FBO_DEFAULT)
	{
		glGenFramebuffers(1, &_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

		//_tex = new Texture(SCREEN_RES_X, SCREEN_RES_Y, 0, GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR);

		// generate texture
		glGenTextures(1,&_tex);
		glBindTexture(GL_TEXTURE_2D, _tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_RES_X, SCREEN_RES_Y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		// attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tex, 0);

		glGenRenderbuffers(1, &_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_RES_X, SCREEN_RES_Y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);
		if (!checkComplete())
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	if (t == FBO_SHADOWPASS)
	{
		//create Frame Buffer
		glGenFramebuffers(1, &_framebuffer);
		const unsigned int W = 2048U, H = 2048U;
		//_tex = new Texture(W, H, 31, GL_TEXTURE_2D, GL_CLAMP_TO_EDGE, GL_NEAREST);

		// Create texture for the depth map to write to
		glGenTextures(1, &_tex);
		glBindTexture(GL_TEXTURE_2D, _tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			W, H, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		/*float borderColor[4] = { 1.0F, 1.0F, 1.0F, 1.0F };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);*/
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, {255, 255, 255, 255});

		//// attach texture to the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _tex, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

FBO::~FBO()
{
	glDeleteFramebuffers(1, &_framebuffer);

}

bool FBO::checkComplete()
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void FBO::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

}



void FBO::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
