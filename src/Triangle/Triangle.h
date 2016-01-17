#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>

#include "object.h"
#include "AntTweakBar.h"

namespace byhj
{
	class Triangle : public Object
	{
	public:

	Triangle();
	~Triangle();

	void init_buffer();
	void init_vertexArray();
	void init_shader();

	private:

	};
}
#endif