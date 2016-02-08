#include "Plane.h"
#include "textureMgr.h"

namespace byhj {

	//left bottom   right bottom    left top            right top
	static const GLenum wrapModes[] ={ GL_CLAMP_TO_EDGE,  GL_REPEAT,  GL_CLAMP_TO_BORDER,  GL_MIRRORED_REPEAT };
	static const GLfloat offsets[] =
	{
		-0.5f, -0.5f,
		0.5f, -0.5f,
		-0.5f,  0.5f,
		0.5f,  0.5f
	};

	Plane::Plane()
	{

	}

	Plane::~Plane()
	{

	}

	void Plane::init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
		init_texture();
	}

	void Plane::update()
	{

	}

	void Plane::render()
	{
		glUseProgram(m_program);

		
		glBindTexture(GL_TEXTURE_2D, tex);
		glActiveTexture(GL_TEXTURE0);
		for (int i = 0; i != 4; ++i)
		{
			//We move the rectangle to four corner
			glUniform2fv(m_offsetLoc, 1, &offsets[i * 2]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModes[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModes[i]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glUseProgram(0);
	}

	void Plane::shutdown()
	{
		glDeleteProgram(m_program);
	}

	void Plane::init_buffer()
	{

	}

	void Plane::init_vertexArray()
	{

	}

	void Plane::init_shader()
	{
		PlaneShader.init();
		PlaneShader.attach(GL_VERTEX_SHADER, "plane.vert");
		PlaneShader.attach(GL_FRAGMENT_SHADER, "plane.frag");
		PlaneShader.link();
		PlaneShader.info();
		m_program = PlaneShader.getProgram();
		m_tex = glGetUniformLocation(m_program, "tex");
		m_offsetLoc = glGetUniformLocation(m_program, "u_offset");

	}
#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
	static const GLubyte tex_data[] =
	{
		B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B,
		B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B,
		B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B,
		B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B,
	};
#undef B
#undef W
	void Plane::init_texture()
	{
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		//internal foramt 
		glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA32F, 8, 8);

		glTexSubImage2D(GL_TEXTURE_2D,
			0,
			0, 0,
			8, 8,
			GL_RGBA, GL_UNSIGNED_BYTE,
			tex_data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//We set the border color for wrapping
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &glm::vec4(0.5f, 0.0f, 0.5f, 1.0f)[0]);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
