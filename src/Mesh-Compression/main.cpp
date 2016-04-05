#include "model.h"

int main(int argc, char * argv[])
{
#ifdef TEST_DYNAMIC_VECTOR_ENCODING
    return testDynamicVectorCompression();
#else
    return testIFSCompression(argc, argv);
#endif
}

