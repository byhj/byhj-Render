#include  "Triangle.h"

namespace byhj
{

	Vertex get
	void Triangle::init_buffer()
	{

		m_VertexData.push_back( Vertex( glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) ) ); 
		m_VertexData.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		m_VertexData.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

			for (int i = 0; i != 3; ++i) {
				m_IndexData.push_back(i);
			}


	}
}