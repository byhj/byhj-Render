#include  "Triangle.h"

namespace byhj
{

	GLfloat* Triangle::getVertexData() 
	{
		return &m_VertexData[0];
	}

	GLuint * Triangle::getIndexData()
	{

		return &m_IndexData[0];
	}

	GLsizei Triangle::getVertexSize()
	{
		auto var = m_VertexData.size() * sizeof(GLfloat);
		return var;
	}
	GLsizei Triangle::getIndexSize()
	{
		return m_IndexData.size() * sizeof(GLuint);
	}

	void Triangle::init_buffer()
	{

		m_VertexData = { -0.5f, -0.5f, 0.0f,   0.5f, -0.5f, 0.0f,   0.0f, 0.5f, 0.0f };

			for (int i = 0; i != 3; ++i) {
				m_IndexData.push_back(i);
			}

	}
}