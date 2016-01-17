#include "Geometry.h"

const float Infinity = FLT_MAX;
const float Pi       = 3.1415926535f;

namespace byhj
{
		void Geometry::createCube(float width, float height, float depth, d3d::MeshData &mesh)
		{
			d3d::Vertex vertex[24];

			float w2 = 0.5f * width;
			float h2 = 0.5f * height;
			float d2 = 0.5f * depth;

			//                    Position         Normal          TexCoord
			// Fill in the front face vertex data.
			vertex[0] = d3d::Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
			vertex[1] = d3d::Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
			vertex[2] = d3d::Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
			vertex[3] = d3d::Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

			// Fill in the back face vertex data.
			vertex[4] = d3d::Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
			vertex[5] = d3d::Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
			vertex[6] = d3d::Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
			vertex[7] = d3d::Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

			// Fill in the top face vertex data.
			vertex[8]  = d3d::Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			vertex[9]  = d3d::Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			vertex[10] = d3d::Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			vertex[11] = d3d::Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the bottom face vertex data.
			vertex[12] = d3d::Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
			vertex[13] = d3d::Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
			vertex[14] = d3d::Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
			vertex[15] = d3d::Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

			// Fill in the left face vertex data.
			vertex[16] = d3d::Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			vertex[17] = d3d::Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			vertex[18] = d3d::Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			vertex[19] = d3d::Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the right face vertex data.
			vertex[20] = d3d::Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			vertex[21] = d3d::Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			vertex[22] = d3d::Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			vertex[23] = d3d::Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			mesh.VertexData.assign(vertex, vertex + 24);

			//////////////////////////Index////////////////////////
			UINT index[36];
			// Fill in the front face index data
			index[0] = 0; index[1] = 1; index[2] = 2;
			index[3] = 0; index[4] = 2; index[5] = 3;

			// Fill in the back face index data
			index[6] = 4; index[7]  = 5; index[8]  = 6;
			index[9] = 4; index[10] = 6; index[11] = 7;

			// Fill in the top face index data
			index[12] = 8; index[13] =  9; index[14] = 10;
			index[15] = 8; index[16] = 10; index[17] = 11;

			// Fill in the bottom face index data
			index[18] = 12; index[19] = 13; index[20] = 14;
			index[21] = 12; index[22] = 14; index[23] = 15;

			// Fill in the left face index data
			index[24] = 16; index[25] = 17; index[26] = 18;
			index[27] = 16; index[28] = 18; index[29] = 19;

			// Fill in the right face index data
			index[30] = 20; index[31] = 21; index[32] = 22;
			index[33] = 20; index[34] = 22; index[35] = 23;

			mesh.IndexData.assign(index, index + 36);
		}

		void Geometry::createCube(float width, float height, float depth, ogl::MeshData &mesh)
		{
			ogl::Vertex vertex[24];

			float w2 = 0.5f * width;
			float h2 = 0.5f * height;
			float d2 = 0.5f * depth;

			//                    Position         Normal          TexCoord
			// Fill in the front face vertex data.
			vertex[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
			vertex[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
			vertex[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
			vertex[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

			// Fill in the back face vertex data.
			vertex[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
			vertex[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
			vertex[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
			vertex[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

			// Fill in the top face vertex data.
			vertex[8]  = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			vertex[9]  = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			vertex[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			vertex[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the bottom face vertex data.
			vertex[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
			vertex[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
			vertex[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
			vertex[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

			// Fill in the left face vertex data.
			vertex[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			vertex[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			vertex[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			vertex[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the right face vertex data.
			vertex[20] = ogl::Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			vertex[21] = ogl::Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			vertex[22] = ogl::Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			vertex[23] = ogl::Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			mesh.VertexData.assign(vertex, vertex + 24);

			//////////////////////////Index////////////////////////
			UINT index[36];
			// Fill in the front face index data
			index[0] = 0; index[1] = 1; index[2] = 2;
			index[3] = 0; index[4] = 2; index[5] = 3;

			// Fill in the back face index data
			index[6] = 4; index[7]  = 5; index[8]  = 6;
			index[9] = 4; index[10] = 6; index[11] = 7;

			// Fill in the top face index data
			index[12] = 8; index[13] =  9; index[14] = 10;
			index[15] = 8; index[16] = 10; index[17] = 11;

			// Fill in the bottom face index data
			index[18] = 12; index[19] = 13; index[20] = 14;
			index[21] = 12; index[22] = 14; index[23] = 15;

			// Fill in the left face index data
			index[24] = 16; index[25] = 17; index[26] = 18;
			index[27] = 16; index[28] = 18; index[29] = 19;

			// Fill in the right face index data
			index[30] = 20; index[31] = 21; index[32] = 22;
			index[33] = 20; index[34] = 22; index[35] = 23;

			mesh.IndexData.assign(index, index + 36);
		}
		//void CreateSphere(int radius, MeshData &mesh);

		void Geometry::CreatePlane(float width, float depth, MeshData &mesh)
		{

			float w2 = width  * 0.5f;
			float d2 = depth * 0.5f;

			Vertex vertex[4];
			vertex[0] = Vertex(-w2, 0.0f, -d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			vertex[1] = Vertex(-w2, 0.0f, d2, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			vertex[2] = Vertex(w2, 0.0f, d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			vertex[3] = Vertex(w2, 0.0f, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

			mesh.VertexData.assign(vertex, vertex + 4);

			UINT index[6];
			index[0] = 0;
			index[1] = 1;
			index[2] = 2;
			index[3] = 2;
			index[4] = 3;
			index[5] = 4;

			mesh.IndexData.assign(index, index + 6);
		}

		void Geometry::CreateGrid(float width, float depth, UINT m, UINT n, MeshData& mesh)
		{
			UINT vertexCount = m * n;
			UINT faceCount   = (m - 1) * (n - 1) * 2;

			float halfWidth = 0.5f * width;
			float halfDepth = 0.5f * depth;

			float dx = width / (n - 1);
			float dz = depth / (m - 1);
			float du = 1.0f / (n - 1);
			float dv = 1.0f / (m - 1);

			mesh.VertexData.resize(vertexCount);
			for (UINT i = 0; i < m; ++i)
			{
				float z = halfDepth - i * dz;
				for (UINT j = 0; j < n; ++j)
				{
					float x = -halfWidth + j * dx;

					mesh.VertexData[i*n + j].Pos      = XMFLOAT3(x, 0.0f, z);
					mesh.VertexData[i*n + j].Normal   = XMFLOAT3(0.0f, 1.0f, 0.0f);
					mesh.VertexData[i*n + j].Tex.x    = j * du;
					mesh.VertexData[i*n + j].Tex.y    = i * dv;
				}
			}

			mesh.IndexData.resize(faceCount * 3); // 3 indices per face
			UINT k = 0;
			for (UINT i = 0; i < m - 1; ++i)
			{
				for (UINT j = 0; j < n - 1; ++j)
				{
					mesh.IndexData[k]     = i*n + j;
					mesh.IndexData[k + 1] = i*n + j + 1;
					mesh.IndexData[k + 2] = (i + 1)*n + j;

					mesh.IndexData[k + 3] = (i + 1)*n + j;
					mesh.IndexData[k + 4] = i*n + j + 1;
					mesh.IndexData[k + 5] = (i + 1)*n + j + 1;

					k += 6; // next quad
				}
			}
		}


		void Geometry::CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData)
		{
			//
			// Compute the vertices stating at the top pole and moving down the stacks.
			//

			// Poles: note that there will be texture coordinate distortion as there is
			// not a unique point on the texture map to assign to the pole when mapping
			// a rectangular texture onto a sphere.
			Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, 0.0f);
			Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

			meshData.VertexData.push_back(topVertex);

			float phiStep   = XM_PI / stackCount;
			float thetaStep = 2.0f*XM_PI / sliceCount;

			// Compute vertices for each stack ring (do not count the poles as rings).
			for (UINT i = 1; i <= stackCount - 1; ++i)
			{
				float phi = i*phiStep;

				// Vertices of ring.
				for (UINT j = 0; j <= sliceCount; ++j)
				{
					float theta = j*thetaStep;

					Vertex v;

					// spherical to cartesian
					v.Pos.x = radius * sinf(phi) * cosf(theta);
					v.Pos.y = radius * cosf(phi);
					v.Pos.z = radius * sinf(phi) * sinf(theta);

					XMVECTOR p = XMLoadFloat3(&v.Pos);
					XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

					v.Tex.x = theta / XM_2PI;
					v.Tex.y = phi / XM_PI;

					meshData.VertexData.push_back(v);
				}
			}
			// Compute indices for top stack.  The top stack was written first to the vertex buffer
			// and connects the top pole to the first ring.
			//

			for (UINT i = 1; i <= sliceCount; ++i)
			{
				meshData.IndexData.push_back(0);
				meshData.IndexData.push_back(i + 1);
				meshData.IndexData.push_back(i);
			}

			//
			// Compute indices for inner stacks (not connected to poles).
			//

			// Offset the indices to the index of the first vertex in the first ring.
			// This is just skipping the top pole vertex.
			UINT baseIndex = 1;
			UINT ringVertexCount = sliceCount + 1;
			for (UINT i = 0; i < stackCount - 2; ++i)
			{
				for (UINT j = 0; j < sliceCount; ++j)
				{
					meshData.IndexData.push_back(baseIndex + i*ringVertexCount + j);
					meshData.IndexData.push_back(baseIndex + i*ringVertexCount + j + 1);
					meshData.IndexData.push_back(baseIndex + (i + 1)*ringVertexCount + j);

					meshData.IndexData.push_back(baseIndex + (i + 1)*ringVertexCount + j);
					meshData.IndexData.push_back(baseIndex + i*ringVertexCount + j + 1);
					meshData.IndexData.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
				}
			}

			//
			// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
			// and connects the bottom pole to the bottom ring.
			//

			// South pole vertex was added last.
			UINT southPoleIndex = (UINT)meshData.VertexData.size() - 1;

			// Offset the indices to the index of the first vertex in the last ring.
			baseIndex = southPoleIndex - ringVertexCount;

			for (UINT i = 0; i < sliceCount; ++i)
			{
				meshData.IndexData.push_back(southPoleIndex);
				meshData.IndexData.push_back(baseIndex + i);
				meshData.IndexData.push_back(baseIndex + i + 1);
			}
		}

		void Geometry::Subdivide(MeshData& meshData)
		{
			// Save a copy of the input geometry.
			MeshData inputCopy = meshData;

			meshData.VertexData.resize(0);
			meshData.IndexData.resize(0);

			//       v1
			//       *
			//      / \
										//     /   \
							//  m0*-----*m1
//   / \   / \
							//  /   \ /   \
							// *-----*-----*
// v0    m2     v2

			UINT numTris = inputCopy.IndexData.size() / 3;
			for (UINT i = 0; i < numTris; ++i)
			{
				Vertex v0 = inputCopy.VertexData[inputCopy.IndexData[i * 3 + 0]];
				Vertex v1 = inputCopy.VertexData[inputCopy.IndexData[i * 3 + 1]];
				Vertex v2 = inputCopy.VertexData[inputCopy.IndexData[i * 3 + 2]];

				//
				// Generate the midpoints.
				//

				Vertex m0, m1, m2;

				// For subdivision, we just care about the position component.  We derive the other
				// vertex components in CreateGeosphere.

				m0.Pos = XMFLOAT3(
					0.5f*(v0.Pos.x + v1.Pos.x),
					0.5f*(v0.Pos.y + v1.Pos.y),
					0.5f*(v0.Pos.z + v1.Pos.z));

				m1.Pos  = XMFLOAT3(
					0.5f*(v1.Pos.x + v2.Pos.x),
					0.5f*(v1.Pos.y + v2.Pos.y),
					0.5f*(v1.Pos.z + v2.Pos.z));

				m2.Pos = XMFLOAT3(
					0.5f*(v0.Pos.x + v2.Pos.x),
					0.5f*(v0.Pos.y + v2.Pos.y),
					0.5f*(v0.Pos.z + v2.Pos.z));

				//
				// Add new geometry.
				//

				meshData.VertexData.push_back(v0); // 0
				meshData.VertexData.push_back(v1); // 1
				meshData.VertexData.push_back(v2); // 2
				meshData.VertexData.push_back(m0); // 3
				meshData.VertexData.push_back(m1); // 4
				meshData.VertexData.push_back(m2); // 5

				meshData.IndexData.push_back(i * 6 + 0);
				meshData.IndexData.push_back(i * 6 + 3);
				meshData.IndexData.push_back(i * 6 + 5);

				meshData.IndexData.push_back(i * 6 + 3);
				meshData.IndexData.push_back(i * 6 + 4);
				meshData.IndexData.push_back(i * 6 + 5);

				meshData.IndexData.push_back(i * 6 + 5);
				meshData.IndexData.push_back(i * 6 + 4);
				meshData.IndexData.push_back(i * 6 + 2);

				meshData.IndexData.push_back(i * 6 + 3);
				meshData.IndexData.push_back(i * 6 + 1);
				meshData.IndexData.push_back(i * 6 + 4);
			}
		}


		void Geometry::CreateGeosphere(float radius, UINT numSubdivisions, MeshData& meshData)
		{
			// Put a cap on the number of subdivisions.
			numSubdivisions = numSubdivisions < 5u ? numSubdivisions : 5u;

			// Approximate a sphere by tessellating an icosahedron.

			const float X = 0.525731f;
			const float Z = 0.850651f;

			XMFLOAT3 pos[12] =
			{
				XMFLOAT3(-X, 0.0f, Z), XMFLOAT3(X, 0.0f, Z),
				XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),
				XMFLOAT3(0.0f, Z, X), XMFLOAT3(0.0f, Z, -X),
				XMFLOAT3(0.0f, -Z, X), XMFLOAT3(0.0f, -Z, -X),
				XMFLOAT3(Z, X, 0.0f), XMFLOAT3(-Z, X, 0.0f),
				XMFLOAT3(Z, -X, 0.0f), XMFLOAT3(-Z, -X, 0.0f)
			};

			DWORD k[60] =
			{
				1, 4, 0, 4, 9, 0, 4, 5, 9, 8, 5, 4, 1, 8, 4,
				1, 10, 8, 10, 3, 8, 8, 3, 5, 3, 2, 5, 3, 7, 2,
				3, 10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0,
				10, 1, 6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7
			};

			meshData.VertexData.resize(12);
			meshData.IndexData.resize(60);

			for (UINT i = 0; i < 12; ++i)
				meshData.VertexData[i].Pos = pos[i];

			for (UINT i = 0; i < 60; ++i)
				meshData.IndexData[i] = k[i];

			for (UINT i = 0; i < numSubdivisions; ++i)
				Subdivide(meshData);

			// Project vertices onto sphere and scale.
			for (UINT i = 0; i < meshData.VertexData.size(); ++i)
			{
				// Project onto unit sphere.
				XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&meshData.VertexData[i].Pos));

				// Project onto sphere.
				XMVECTOR p = radius*n;

				XMStoreFloat3(&meshData.VertexData[i].Pos, p);
				XMStoreFloat3(&meshData.VertexData[i].Normal, n);

				// Derive texture coordinates from spherical coordinates.
				float theta = AngleFromXY(meshData.VertexData[i].Pos.x, meshData.VertexData[i].Pos.z);

				float phi = acosf(meshData.VertexData[i].Pos.y / radius);

				meshData.VertexData[i].Tex.x = theta / XM_2PI;
				meshData.VertexData[i].Tex.y = phi / XM_PI;
			}
		}

		void Geometry::CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData)
		{
			meshData.VertexData.clear();
			meshData.IndexData.clear();

			//
			// Build Stacks.
			// 

			float stackHeight = height / stackCount;

			// Amount to increment radius as we move up each stack level from bottom to top.
			float radiusStep = (topRadius - bottomRadius) / stackCount;

			UINT ringCount = stackCount + 1;

			// Compute vertices for each stack ring starting at the bottom and moving up.
			for (UINT i = 0; i < ringCount; ++i)
			{
				float y = -0.5f*height + i*stackHeight;
				float r = bottomRadius + i*radiusStep;

				// vertices of ring
				float dTheta = 2.0f*XM_PI / sliceCount;
				for (UINT j = 0; j <= sliceCount; ++j)
				{
					Vertex vertex;

					float c = cosf(j*dTheta);
					float s = sinf(j*dTheta);

					vertex.Pos = XMFLOAT3(r*c, y, r*s);

					vertex.Tex.x = (float)j / sliceCount;
					vertex.Tex.y = 1.0f - (float)i / stackCount;

					// Cylinder can be parameterized as follows, where we introduce v
					// parameter that goes in the same direction as the v tex-coord
					// so that the bitangent goes in the same direction as the v tex-coord.
					//   Let r0 be the bottom radius and let r1 be the top radius.
					//   y(v) = h - hv for v in [0,1].
					//   r(v) = r1 + (r0-r1)v
					//
					//   x(t, v) = r(v)*cos(t)
					//   y(t, v) = h - hv
					//   z(t, v) = r(v)*sin(t)
					// 
					//  dx/dt = -r(v)*sin(t)
					//  dy/dt = 0
					//  dz/dt = +r(v)*cos(t)
					//
					//  dx/dv = (r0-r1)*cos(t)
					//  dy/dv = -h
					//  dz/dv = (r0-r1)*sin(t)

					// This is unit length.
					XMFLOAT3 tangent = XMFLOAT3(-s, 0.0f, c);

					float dr = bottomRadius - topRadius;
					XMFLOAT3 bitangent(dr*c, -height, dr*s);

					XMVECTOR T = XMLoadFloat3(&tangent);
					XMVECTOR B = XMLoadFloat3(&bitangent);
					XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
					XMStoreFloat3(&vertex.Normal, N);

					meshData.VertexData.push_back(vertex);
				}
			}

			// Add one because we duplicate the first and last vertex per ring
			// since the texture coordinates are different.
			UINT ringVertexCount = sliceCount + 1;

			// Compute indices for each stack.
			for (UINT i = 0; i < stackCount; ++i)
			{
				for (UINT j = 0; j < sliceCount; ++j)
				{
					meshData.IndexData.push_back(i*ringVertexCount + j);
					meshData.IndexData.push_back((i + 1)*ringVertexCount + j);
					meshData.IndexData.push_back((i + 1)*ringVertexCount + j + 1);

					meshData.IndexData.push_back(i*ringVertexCount + j);
					meshData.IndexData.push_back((i + 1)*ringVertexCount + j + 1);
					meshData.IndexData.push_back(i*ringVertexCount + j + 1);
				}
			}

			BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
			BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
		}

		void Geometry::BuildCylinderTopCap(float bottomRadius, float topRadius, float height,
			UINT sliceCount, UINT stackCount, MeshData& meshData)
		{
			UINT baseIndex = (UINT)meshData.VertexData.size();

			float y = 0.5f*height;
			float dTheta = 2.0f*XM_PI / sliceCount;

			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for (UINT i = 0; i <= sliceCount; ++i)
			{
				float x = topRadius*cosf(i*dTheta);
				float z = topRadius*sinf(i*dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / height + 0.5f;
				float v = z / height + 0.5f;

				meshData.VertexData.push_back(Vertex(x, y, z, 0.0f, 1.0f, 0.0f, u, v));
			}

			// Cap center vertex.
			meshData.VertexData.push_back(Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f));

			// Index of center vertex.
			UINT centerIndex = (UINT)meshData.VertexData.size() - 1;

			for (UINT i = 0; i < sliceCount; ++i)
			{
				meshData.IndexData.push_back(centerIndex);
				meshData.IndexData.push_back(baseIndex + i + 1);
				meshData.IndexData.push_back(baseIndex + i);
			}
		}

		void Geometry::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height,
			UINT sliceCount, UINT stackCount, MeshData& meshData)
		{
			// 
			// Build bottom cap.
			//

			UINT baseIndex = (UINT)meshData.VertexData.size();
			float y = -0.5f*height;

			// vertices of ring
			float dTheta = 2.0f*XM_PI / sliceCount;
			for (UINT i = 0; i <= sliceCount; ++i)
			{
				float x = bottomRadius*cosf(i*dTheta);
				float z = bottomRadius*sinf(i*dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / height + 0.5f;
				float v = z / height + 0.5f;

				meshData.VertexData.push_back(Vertex(x, y, z, 0.0f, -1.0f, 0.0f, u, v));
			}

			// Cap center vertex.
			meshData.VertexData.push_back(Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f));

			// Cache the index of center vertex.
			UINT centerIndex = (UINT)meshData.VertexData.size() - 1;

			for (UINT i = 0; i < sliceCount; ++i)
			{
				meshData.IndexData.push_back(centerIndex);
				meshData.IndexData.push_back(baseIndex + i);
				meshData.IndexData.push_back(baseIndex + i + 1);
			}
		}

	}