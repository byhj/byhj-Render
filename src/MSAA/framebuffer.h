#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <gl/glew.h>

namespace byhj
{

class FrameBuffer
{
public:
	FrameBuffer()  = default;
	~FrameBuffer() = default;


	void Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); 
	}
	GLuint GetFbo()
	{
		return framebuffer;
	}

	void init(int sw, int sh);
	// Generates a texture that is suited for attachments to a framebuffer
	GLuint generateMultiSampleTexture(GLuint samples);

private:
	int width, height;
	GLuint textureColorBufferMultiSampled;
	GLuint framebuffer;
};


}
#endif