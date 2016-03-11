#ifndef Scene_H
#define Scene_H

#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/model.h"
#include "ogl/camera.h"
#include "ogl/model.h"

namespace byhj
{

	class Scene
	{
	public:
		Scene(){}
		~Scene(){}

	public:
		void Init();
		void Render(const ogl::MvpMatrix &matrix, const ogl::Camera &camera);
		void Shutdown();
		void ChangeShadow();
		void init_fbo(int sw, int sh);

	private:
		void init_buffer();
		void init_vertexArray();
		void init_shader();
		void init_texture();

		void RenderQuad();
		void RenderCube();

		GLuint cubeVAO, planeVAO, cubeVBO, planeVBO;

		GLuint model_prog, light_prog, blur_prog, ssao_prog;
		ogl::Shader modelShader, lightShader, blurShader, ssaoShader;

		ogl::Model  cyborg;
		GLuint gBuffer;
        GLuint gPositionDepth, gNormal, gAlbedo;
		GLuint noiseTexture;
		GLuint ssaoFBO, ssaoBlurFBO;
		GLuint ssaoColorBuffer, ssaoColorBufferBlur;
	};


}

#endif