#include "sample.h"

// #ifdef _DEBUG
// #pragma comment( linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
// #endif

// #ifdef _DEBUG
// #include <vld.h>
// #endif

int main(int argc, char **argv)
{
	auto sample = std::make_shared<byhj::Sample>();
	sample->init();
	sample->run();
	sample->end();

	return 0;
}