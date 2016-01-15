#ifndef shader_H
#define shader_H

#include <string>

namespace byhj
{
	class Shader
	{
	public:
	  Shader();
	  Shader(std::string name);
	  virtual ~Shader();

	protected:
		std::string m_name;
	};
}
#endif