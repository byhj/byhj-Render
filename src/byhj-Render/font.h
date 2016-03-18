#ifndef _Font_H_
#define _Font_H_

#include <iostream>

namespace byhj
{
	class Font {
	public:
		Font(); 
		virtual ~Font();

	protected:
		std::string m_dir;
	};
}
#endif