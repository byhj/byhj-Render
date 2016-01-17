#ifndef Font_H
#define Font_H

#include <iostream>

namespace byhj
{
	class Font {
	public:
		Font(); 
		~Font();

	private:
		std::string m_dir;
	};
}
#endif