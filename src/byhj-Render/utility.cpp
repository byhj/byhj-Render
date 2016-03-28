#include "utility.h"

namespace byhj {


bool test(bool expression, std::string desc, int line, char* file)
{
	std::cout << desc << " On " << line << " " << file << std::endl;
	return expression;
}

}