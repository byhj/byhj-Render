#ifndef OGLDEBUG_H
#define OGLDEBUG_H

#include <memory>

namespace byhj
{

	class OGLDebug
	{
	public:
		OGLDebug() 	 = default;
		~OGLDebug()	 = default;

		static std::shared_ptr<OGLDebug> getInstance();
		void debug();

	private:


	};
}

#endif
