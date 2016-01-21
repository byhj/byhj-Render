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
   
	private:
		void init_buffer();

	};
}
#endif