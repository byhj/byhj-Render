#ifndef Geometry_H
#define Geometry_H

#include <windows.h>
#include <vector>
#include <algorithm>
#include <DirectXMath.h>
#include <gl/glew.h>
#include <glm/glm.hpp>

using namespace DirectX;

namespace byhj
{
	namespace d3d {
		struct Vertex	{
			Vertex();
			Vertex(const XMFLOAT3 &pos, const XMFLOAT3 &normal, const XMFLOAT2 &tc)
				:position(pos), normal(normal), texcoord(tc) {}

			Vertex(float px, float py, float pz,
				float nx, float ny, float nz,
				float u, float v) : position(px, py, pz), normal(nx, ny, nz), texcoord(u, v) {}

			~Vertex();

			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 texcoord;
		};

		struct MeshData
		{
			std::vector<Vertex> VertexData;
			std::vector<UINT> IndexData;
		};

	}//namespace d3d

	namespace ogl {
		struct Vertex {

			Vertex();
			Vertex(const  glm::vec3 &pos, const glm::vec3 &normal, const glm::vec2 &tc)
				:position(pos), normal(normal), texcoord(tc) {}

			Vertex(float px, float py, float pz,
				float nx, float ny, float nz,
				float u, float v) : position(px, py, pz), normal(nx, ny, nz), texcoord(u, v) {}

			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texcoord;
		};

		struct MeshData {
			std::vector<Vertex> VertexData;
			std::vector<GLuint> IndexData;
		};
	}

	class Geometry {
	public:
		Geometry(); 
		~Geometry();

		void createPlane(float width, float height, d3d::MeshData &mesh);
		void createPlane(float width, float height, ogl::MeshData &mesh);

		void createCube(float widht, float height, float depth, d3d::MeshData &mesh);
		void createCube(float widht, float height, float depth, ogl::MeshData &mesh);

		void createGrid(float width, float depth, UINT m, UINT n, d3d::MeshData& mesh);
		void createGrid(float width, float depth, UINT m, UINT n, ogl::MeshData& mesh);

		/// Creates a sphere centered at the origin with the given radius.  The
		/// slices and stacks parameters control the degree of tessellation.

		void createSphere(float radius, UINT sliceCount, UINT stackCount, d3d::MeshData& mesh);
		void createSphere(float radius, UINT sliceCount, UINT stackCount, ogl::MeshData& mesh);


		// void createCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, d3d::MeshData& mesh);
		// void createCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, ogl::MeshData& mesh);
		// 

	private:

	};
}
#endif