#include "sample.h"

int main()
{
	byhj::Sample *sample = new byhj::Sample();
	sample->run();
	delete sample;

	return 0;
}