#ifndef OGLFont_H
#define OGLFont_H

#include "font.h"
#include "ogl/oglShader.h"

#include <freetype/ft2build.h>
#include FT_FREETYPE_H

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

namespace byhj {

	/// Holds all state information relevant to a character as loaded using FreeType
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

		void init();
		void render(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

	private:
		void init_shader();
		void init_buffer();

		OGLShader m_FontShader;

		std::map<GLchar, Character> Characters;
		GLuint m_vao, m_vbo;
    	GLuint m_program;
	};
}
#endif