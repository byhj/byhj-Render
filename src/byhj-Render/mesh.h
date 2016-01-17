#ifndef Mesh_H
#define Mesh_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace byhj
{
	struct Vertex {
	   glm::vec3 position;
	   glm::vec3 normal;
	   glm::vec2 texcoord;
	};

	class Mesh {
	  public:
		  Mesh();
		  Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices)
		  {
			  m_VertexData = vertices;
			  m_IndexData  = indices;
		  }
		  Mesh(const Mesh &mesh);
		  ~Mesh();

	  private:
		  std::vector<Vertex> m_VertexData;
		  std::vector<GLuint> m_IndexData;
	};

}

#endif