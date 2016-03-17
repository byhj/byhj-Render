#ifndef Bloom_H
#define Bloom_H

#include "ogl/oglShader.h"
#include "ogl/oglModel.h"
#include "ogl/oglCamera.h"

namespace byhj {

   class Bloom {
     
   public:
	   void init();
	   void update();
	   void render(const OGLCamera &camera);
	   void shutdown();

   private:
	   void render_plane();

	   void init_buffer();
	   void init_shader();
	   void init_texture();
	   void init_vertexArray();
	   void init_fbo();

	   struct UniformLoc
	   {
		   GLuint model;
		   GLuint view;
		   GLuint proj;
	   }uniform_loc;
	   OGLModel m_model;
	   
	   GLuint m_diffuseTex;
	   GLuint m_glowTex;

	   GLuint vao, vbo;
	   GLfloat exposure;
	   GLuint model_loc, view_loc, proj_loc;
	   GLuint intensity_loc;
	   GLuint m_bloomProgram, m_blurProgram, m_sceneProgram;

	   GLuint m_blurFbos[2];
	   GLuint m_blurTexs[2];
	   GLuint m_bloomTexs[2];
	   GLuint m_bloomFbo;
	   GLuint m_depthRbo;

	   OGLShader m_blurShader = "Blur shader";
	   OGLShader m_sceneShader = "Scene Shader";
	   OGLShader m_bloomShader = "Bloom Shader";
   };
}
#endif