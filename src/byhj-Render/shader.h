#ifndef shader_H
#define shader_H

#include <string>

namespace byhj
{
	class Shader
	{
	public:
	  Shader() = default;
	  Shader(std::string name);
	  virtual ~Shader() = default;

	protected:
		std::string m_name;
	};
}
#endif