#include "utility.h"

namespace byhj {


bool checkOGLUniform(bool expression, std::string desc, int line, char* file, bool ignore)
{
	if (expression != true) {
		std::cout << desc << " On " << line << " " << file << std::endl;
	}
	return true;
}

}