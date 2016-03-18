#ifndef OGLFont_H
#define OGLFont_H

#include "font.h"
#include "ogl/oglShader.h"
#include "shaderMgr.h"

#include <freetype/ft2build.h>
#include FT_FREETYPE_H

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

#pragma  comment(lib, "freetype26d")

namespace byhj {

	// Holds all state information relevant to a character as loaded using FreeType
	struct Character {
		GLuint TextureID;   // ID handle of the glyph texture
		glm::ivec2 Size;    // Size of glyph
		glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
		GLuint Advance;    // Horizontal offset to advance to next glyph
	};

	class OGLFont : public Font {
	public:
		OGLFont();
		~OGLFont();

		void init(GLfloat sw, GLfloat sh, std::string fontFile = "consolai.ttf");
		void render(std::string text, GLfloat x, GLfloat y, GLfloat scale = 0.5f, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f) );
		void update();
		void shutdown();

	private:
		void init_shader();
		void init_buffer();
		void init_vertexArray();

		OGLShader m_FontShader = "Font Shader";
		GLfloat m_sw, m_sh;
		std::map<GLchar, Character> Characters;
		GLuint m_vao, m_vbo;
    	GLuint m_program;
		std::string m_FontFile;
	};
}
#endif