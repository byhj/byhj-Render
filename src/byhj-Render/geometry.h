#ifndef Geometry_H
#define Geometry_H

#include <windows.h>
#include <vector>
#include <algorithm>
#include <DirectXMath.h>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <memory>

using namespace DirectX;

namespace byhj
{
		struct D3DVertex	{
			D3DVertex() = default;
			D3DVertex(const XMFLOAT3 &pos, const XMFLOAT3 &normal, const XMFLOAT2 &tc)
				:position(pos), normal(normal), texcoord(tc) {}

			D3DVertex(float px, float py, float pz,
				float nx, float ny, float nz,
				float u, float v) : position(px, py, pz), normal(nx, ny, nz), texcoord(u, v) {}

			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 texcoord;
		};

		struct D3DMeshData
		{
			std::vector<D3DVertex> VertexData;
			std::vector<UINT> IndexData;
		};

		struct OGLVertex {

			OGLVertex() = default;
			OGLVertex(const  glm::vec3 &pos, const glm::vec3 &normal, const glm::vec2 &tc)
				:position(pos), normal(normal), texcoord(tc) {}

			OGLVertex(float px, float py, float pz,
				float nx, float ny, float nz,
				float u, float v) : position(px, py, pz), normal(nx, ny, nz), texcoord(u, v) {}

			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texcoord;
		};

		struct OGLMeshData {
			std::vector<OGLVertex> VertexData;
			std::vector<GLuint> IndexData;
		}; 

	class Geometry {
	public:
		Geometry() = default; 
		~Geometry() = default;

		static std::shared_ptr<Geometry> getInstance();

		void createPlane(float width, float height, D3DMeshData &mesh);
		void createPlane(float width, float height, OGLMeshData &mesh);

		void createCube(float widht, float height, float depth, D3DMeshData &mesh);
		void createCube(float widht, float height, float depth, OGLMeshData &mesh);

		void createGrid(float width, float depth, UINT m, UINT n, D3DMeshData& mesh);
		void createGrid(float width, float depth, UINT m, UINT n, OGLMeshData& mesh);

		/// Creates a sphere centered at the origin with the given radius.  The
		/// slices and stacks parameters control the degree of tessellation.

		void createSphere(float radius, UINT sliceCount, UINT stackCount, D3DMeshData& mesh);
		void createSphere(float radius, UINT sliceCount, UINT stackCount, OGLMeshData& mesh);


		// void createCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, D3DMeshData& mesh);
		// void createCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, OGLMeshData& mesh);
		// 

	private:

	};
}
#endif