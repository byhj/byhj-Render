#ifndef Object_H
#define Object_H

#include <gl/glew.h>
#include <vector>

namespace byhj
{
	class Object
	{
	public:

		Object() = default;
		virtual ~Object() = default;

		const GLfloat *getVertexData()  const;
		const GLuint  *getIndexData()   const;
		GLuint   getVertexCount() const;
		GLuint   getIndexCount()  const;
		GLsizei  getVertexSize()  const;
		GLsizei  getIndexSize()   const;

	protected:
		std::vector<GLfloat> m_VertexData;
		std::vector<GLuint> m_IndexData;
		GLuint m_VertexCount = 0;
		GLuint m_IndexCount  = 0;
	};
}
#endif