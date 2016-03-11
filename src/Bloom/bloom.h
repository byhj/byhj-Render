#ifndef Bloom_H
#define Bloom_H

#include "ogl/oglShader.h"
#include "model.h"
#include "ogl/oglCamera.h"

namespace byhj {

   class Bloom {
     
   public:
	   void init();
	   void update();
	   void render(const OGLCamera &camera);
	   void shutdown();

   private:
	   void init_buffer();
	   void init_shader();
	   void init_texture();
	   struct UniformLoc
	   {
		   GLuint model;
		   GLuint view;
		   GLuint proj;
	   }uniform_loc;
	   GLuint m_program;
	   Model m_model;
	   
	   GLuint m_diffuseTex;
	   GLuint m_glowTex;

	   OGLShader m_bloomShader = "Bloom Shader";
   };
}
#endif