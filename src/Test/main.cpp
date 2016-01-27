#include "sample.h"

#ifdef _DEBUG
#include <vld.h>
#endif

int main()
{
	auto sample = std::make_shared<byhj::Sample>();
	sample->init();
	sample->run();
	sample->end();

	return 0;
}