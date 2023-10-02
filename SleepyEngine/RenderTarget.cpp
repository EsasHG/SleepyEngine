#include "RenderTarget.h"
#include <glad/glad.h>
#include <string>
#include <iostream>
namespace Sleepy
{
	RenderTarget::RenderTarget()
	{
		glGenFramebuffers(1, &framebuffer);
		glGenTextures(1, &renderedTexture);
		glGenRenderbuffers(1, &renderbuffer);
		RecreateFramebuffer();
	}
	
	RenderTarget::~RenderTarget()
	{
		glDeleteRenderbuffers(1, &renderbuffer);
		glDeleteTextures(1, &renderedTexture);
		glDeleteFramebuffers(1, &framebuffer);
	
	}
	
	void RenderTarget::RecreateFramebuffer()
	{

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, bufferHeigth, bufferWidth, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bufferHeigth, bufferWidth);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,renderbuffer);

		glBindTexture(GL_TEXTURE_2D, 0);


		GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		std::string errorstring;
		switch (framebufferStatus)
		{
		case GL_FRAMEBUFFER_COMPLETE:
			return;
		case GL_FRAMEBUFFER_UNDEFINED:
			errorstring = "Undefined framebuffer!";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			errorstring = "Incomplete Attachment!";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			errorstring = "Imcomplete Missing Attachment!";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			errorstring = "Incomplete Draw Buffer!";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			errorstring = "Incomplete Read Buffer!";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			errorstring = "Unsupported Framebuffer!";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			errorstring = "Incomplete Multisample!";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			errorstring = "Incomplete Layer Targets!";
			break;
		default:
			errorstring = "Unknown Error";
			break;
		}

		std::cout << "ERROR::FRAMEBUFFER: " << errorstring << std::endl;

	}

}
