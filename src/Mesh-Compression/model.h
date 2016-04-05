
#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "o3dgcCommon.h"
#include "o3dgcVector.h"
#include "o3dgcSC3DMCEncodeParams.h"
#include "o3dgcIndexedFaceSet.h"
#include "o3dgcSC3DMCEncoder.h"
#include "o3dgcSC3DMCDecoder.h"
#include "o3dgcTimer.h"
#include "o3dgcDVEncodeParams.h"
#include "o3dgcDynamicVectorEncoder.h"
#include "o3dgcDynamicVectorDecoder.h"

//#define ADD_FAKE_ANIMATION_WEIGHTS
//#define TEST_DYNAMIC_VECTOR_ENCODING

#ifdef WIN32
#define PATH_SEP "\\"
#else
#define PATH_SEP "/"
#endif

using namespace o3dgc;

class IVec3Cmp
{
public:
	bool operator()(const Vec3<unsigned long> a, const Vec3<unsigned long> b) const
	{
		if (a.X() != b.X())
		{
			return (a.X() > b.X());
		}
		else if (a.Y() != b.Y())
		{
			return (a.Y() > b.Y());
		}
		return (a.Z() > b.Z());
	}
};

class Material
{
public:
	Material(unsigned long id, unsigned long numTriangles, const std::string & name)
	{
		m_id           = id;
		m_numTriangles = numTriangles;
		m_name         = name;
	};
	Material()
	{
		m_id           = 0;
		m_numTriangles = 0;
	};
	unsigned long	m_id;
	unsigned long   m_numTriangles;
	std::string     m_name;
};

bool LoadOBJ(const std::string & fileName,
	std::vector< Vec3<Real> > & upoints,
	std::vector< Vec2<Real> > & utexCoords,
	std::vector< Vec3<Real> > & unormals,
	std::vector< Vec3<unsigned long> > & triangles,
	std::vector< unsigned long > & indexBufferIDs,
	std::vector< Material > & materials,
	std::string & materialLib);

bool LoadIFS(const std::string & fileName,
	std::vector< Vec3<Real> > & points,
	std::vector< Vec2<Real> > & texCoords,
	std::vector< Vec3<Real> > & normals,
	std::vector< Vec3<unsigned long> > & triangles,
	std::vector< unsigned long > & indexBufferIDs);

bool SaveOBJ(const std::string & fileName,
	const std::vector< Vec3<Real> > & points,
	const std::vector< Vec2<Real> > & texCoords,
	const std::vector< Vec3<Real> > & normals,
	const std::vector< Vec3<unsigned long> > & triangles,
	const std::vector< Material > & materials,
	const std::vector< unsigned long > indexBufferIDs,
	const std::string & materialLib);

bool LoadMaterials(const std::string & fileName,
	std::vector< Material > & materials,
	std::string & materialLib);

bool SaveMaterials(const std::string & fileName,
	const std::vector< Material > & materials,
	const std::string & materialLib);
bool SaveIFS(const std::string & fileName,
	const IndexedFaceSet<unsigned long> & ifs);
bool Check(const IndexedFaceSet<unsigned long> & ifs);


int testEncode(const std::string & fileName, int qcoord, 
	           int qtexCoord, int qnormal, O3DGCStreamType streamType);


int testDecode(std::string & fileName);

enum Mode
{
    UNKNOWN = 0,
    ENCODE  = 1,
    DECODE  = 2
};

int testDynamicVectorCompression();
int testIFSCompression(int argc, char * argv[]);
