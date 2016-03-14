#ifndef Scene_H
#define Scene_H

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
#include "ogl/oglModel.h"
#include "ogl/oglCamera.h"


namespace byhj
{

	class Scene
	{
	public:
		Scene(){}
		~Scene(){}

	public:
		void init();
		void render( const OGLCamera &camera);
		void shutdown();
		void changeShadow();


	private:
		void init_buffer();
		void init_vertexArray();
		void init_shader();
		void init_texture();
		void init_fbo();

		void RenderQuad();
		void RenderCube();

		GLuint cubeVAO, planeVAO, cubeVBO, planeVBO;
		GLuint    model_prog, light_prog, blur_prog, ssao_prog;
		OGLShader modelShader, lightShader, blurShader, ssaoShader;
	    OGLModel   m_model;

		GLuint m_gbufferFbo;
        GLuint m_posDepthTex, m_normalTex, m_colorTex;
		GLuint m_depthRbo;

		GLuint m_noiseTex;
		GLuint m_ssaoFbo, m_ssaoBlurFbo;
		GLuint m_ssaoColorTex, m_ssaoColorBlurTex;
	};


}

#endif