#ifndef __KTX_H__
#define __KTX_H__

#include "GL/glew.h"

namespace byhj
{

struct header
{
    unsigned char       identifier[12];
    unsigned int        endianness;
    unsigned int        gltype;
    unsigned int        gltypesize;
    unsigned int        glformat;
    unsigned int        glinternalformat;
    unsigned int        glbaseinternalformat;
    unsigned int        pixelwidth;
    unsigned int        pixelheight;
    unsigned int        pixeldepth;
    unsigned int        arrayelements;
    unsigned int        faces;
    unsigned int        miplevels;
    unsigned int        keypairbytes;
};

union keyvaluepair
{
    unsigned int        size;
    unsigned char       rawbytes[4];
};

GLuint loadKtx(const char * filename);
bool save(const char * filename, unsigned int target, unsigned int tex);

}
#endif /* __SB6KTX_H__ */
