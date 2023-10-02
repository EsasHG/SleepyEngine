#pragma once
namespace Sleepy
{
	class RenderTarget
	{
	public:
		RenderTarget();
		virtual ~RenderTarget();
		void RecreateFramebuffer();

		unsigned int framebuffer = 0; 
		unsigned int renderbuffer = 0;
		unsigned int renderedTexture = 0;
		unsigned int bufferHeigth = 1920;
		unsigned int bufferWidth = 1080;
	};
}

