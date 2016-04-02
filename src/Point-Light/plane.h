#ifndef Plane_H
#define Plane_H

#include "ogl/oglShader.h"
#include "TextureMgr.h"
#include "LightGUI.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ogl/oglEulerCamera.h"

namespace byhj
{

   class Plane
   {
   public:
	   Plane() = default;
	  ~Plane() = default;

	  void init();
	  void update();
	  void render();
	  void shutdown();

   private:
	   void init_buffer();
	   void init_vertexArray();
	   void init_shader();
	   void init_texture();

	   OGLShader m_PlaneShader;

	   LightGui m_LightGUI;

	   struct UniformLoc
	   {
		   GLuint model;
		   GLuint view;
		   GLuint proj;
		   GLuint gamma;
		   GLuint viewPos;
		   GLuint lightPos;
		   GLuint lightColor;
		   GLuint woodTex;
		   GLuint lightModelSub;
		   GLuint lightSub[2];
	   }uniform_loc;

	   GLuint program = 0;
	   GLuint vao = 0;
	   GLuint vbo = 0;
	   GLuint ibo = 0;
	   GLuint woodTexs[2];
	   GLfloat aspect = 1.0f;

   };


}
#endif