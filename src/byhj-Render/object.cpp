#include "object.h"

namespace byhj
{

	Object::Object()
	{

	}
	Object::~Object()
	{

	}
	const GLfloat * Object::getVertexData() const
	{

		return &m_VertexData[0];
	}
	const GLuint  * Object::getIndexData()   const
	{
		return &m_IndexData[0];
	}

	GLuint   Object::getVertexCount() const
	{
		return m_VertexData.size();
	}
	GLuint   Object::getIndexCount()  const
	{
		return m_IndexData.size();
	}
	GLsizei  Object::getVertexSize()  const
	{
		return getVertexCount() * sizeof(GLfloat);
	}
	GLsizei  Object::getIndexSize()   const
	{
		return getIndexSize() * sizeof(GLuint);
	}
}