#include "Geometry.h"

const float Infinity = FLT_MAX;

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
			vertex[0] = ogl::Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
			vertex[1] = ogl::Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
			vertex[2] = ogl::Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
			vertex[3] = ogl::Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

			// Fill in the back face vertex data.
			vertex[4] = ogl::Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
			vertex[5] = ogl::Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
			vertex[6] = ogl::Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
			vertex[7] = ogl::Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

			// Fill in the top face vertex data.
			vertex[8]  = ogl::Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			vertex[9]  = ogl::Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			vertex[10] = ogl::Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			vertex[11] = ogl::Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the bottom face vertex data.
			vertex[12] = ogl::Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
			vertex[13] = ogl::Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
			vertex[14] = ogl::Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
			vertex[15] = ogl::Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

			// Fill in the left face vertex data.
			vertex[16] = ogl::Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			vertex[17] = ogl::Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			vertex[18] = ogl::Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			vertex[19] = ogl::Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

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

		void Geometry::createPlane(float width, float depth, d3d::MeshData &mesh)
		{

			float w2 = width  * 0.5f;
			float d2 = depth * 0.5f;

			ogl::Vertex vertex[4];
			vertex[0] = ogl::Vertex(-w2, 0.0f, -d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			vertex[1] = ogl::Vertex(-w2, 0.0f, d2, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			vertex[2] = ogl::Vertex(w2, 0.0f, d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			vertex[3] = ogl::Vertex(w2, 0.0f, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

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

		void Geometry::createPlane(float width, float depth, ogl::MeshData &mesh)
		{

			float w2 = width  * 0.5f;
			float d2 = depth * 0.5f;

			ogl::Vertex vertex[4];
			vertex[0] = ogl::Vertex(-w2, 0.0f, -d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			vertex[1] = ogl::Vertex(-w2, 0.0f, d2, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			vertex[2] = ogl::Vertex(w2, 0.0f, d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			vertex[3] = ogl::Vertex(w2, 0.0f, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

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

		void Geometry::createGrid(float width, float depth, UINT m, UINT n, d3d::MeshData& mesh)
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

					mesh.VertexData[i*n + j].position      = XMFLOAT3(x, 0.0f, z);
					mesh.VertexData[i*n + j].normal  = XMFLOAT3(0.0f, 1.0f, 0.0f);
					mesh.VertexData[i*n + j].texcoord.x    = j * du;
					mesh.VertexData[i*n + j].texcoord.y    = i * dv;
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
		void Geometry::createGrid(float width, float depth, UINT m, UINT n, ogl::MeshData& mesh)
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

					mesh.VertexData[i*n + j].position = glm::vec3(x, 0.0f, z);
					mesh.VertexData[i*n + j].normal  =  glm::vec3(0.0f, 1.0f, 0.0f);
					mesh.VertexData[i*n + j].texcoord.x    = j * du;
					mesh.VertexData[i*n + j].texcoord.y    = i * dv;
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


		void Geometry::createSphere(float radius, UINT sliceCount, UINT stackCount, d3d::MeshData& meshData)
		{
			//
			// Compute the vertices stating at the top pole and moving down the stacks.
			//

			// Poles: note that there will be texture coordinate distortion as there is
			// not a unique point on the texture map to assign to the pole when mapping
			// a rectangular texture onto a sphere.
			d3d::Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, 0.0f);
			d3d::Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

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

					d3d::Vertex v;

					// spherical to cartesian
					v.position.x = radius * sinf(phi) * cosf(theta);
					v.position.y = radius * cosf(phi);
					v.position.z = radius * sinf(phi) * sinf(theta);

					XMVECTOR p = XMLoadFloat3(&v.position);
					XMStoreFloat3(&v.normal, XMVector3Normalize(p));

					v.texcoord.x = theta / XM_2PI;
					v.texcoord.y = phi / XM_PI;

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

		void Geometry::createSphere(float radius, UINT sliceCount, UINT stackCount, ogl::MeshData& meshData)
		{
			//
			// Compute the vertices stating at the top pole and moving down the stacks.
			//

			// Poles: note that there will be texture coordinate distortion as there is
			// not a unique point on the texture map to assign to the pole when mapping
			// a rectangular texture onto a sphere.
			ogl::Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, 0.0f);
			ogl::Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

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

					ogl::Vertex v;

					// spherical to cartesian
					v.position.x = radius * sinf(phi) * cosf(theta);
					v.position.y = radius * cosf(phi);
					v.position.z = radius * sinf(phi) * sinf(theta);

                    v.normal = v.position;
					glm::normalize(v.normal);

					v.texcoord.x = theta / XM_2PI;
					v.texcoord.y = phi / XM_PI;

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

	}