#include "sample.h"

// #ifdef _DEBUG
// #include <vld.h>
// #endif

int main()
{
	byhj::Sample sample;
	sample.init();
	sample.run();
	sample.end();

	return 0;
}