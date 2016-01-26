#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>
#include <vector>
#include "object.h"
#include "AntTweakBar.h"
#include <glm/glm.hpp>

namespace byhj
{
	class Triangle : public Object
	{
	public:


	Triangle()  = default;
	~Triangle() = default;
   
	GLfloat *getVertexData();
	GLuint  *getIndexData();
	GLsizei getVertexSize();
	GLsizei getIndexSize();

	private:
		void init_buffer();

		std::vector<GLfloat> m_VertexData;
		std::vector<GLuint>  m_IndexData;

	};
}
#endif