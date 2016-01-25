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
	
		struct Vertex {
			Vertex(glm::vec3 pos, glm::vec3 c) : position(pos), color(c) {}
			glm::vec3 position;
			glm::vec3 color;
		};

	Triangle();
	~Triangle();
   
	Vertex *getVertexData();
	GLuint *getIndexData();
	private:
		void init_buffer();

		std::vector<Vertex> m_VertexData;
		std::vector<GLuint>  m_IndexData;

	};
}
#endif