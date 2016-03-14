#include "Scene.h"
#include "windowInfo.h"
#include "textureMgr.h"
#include <random>

GLfloat lerp(GLfloat a, GLfloat b, GLfloat f)
{
	return a + f * (b - a);
}

namespace byhj
{
#pragma region VertexData

	GLfloat planeVertices[] = {
		// Positions        // Texture Coords
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	GLfloat cubeVertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
		// Front face
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
		// Left face
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		// Right face
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
		// Top face
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left     
	};
#pragma endregion

	void Scene::init()
	{
		init_buffer();
		init_vertexArray();
		init_shader();
		init_texture();
		init_fbo();
	}

	// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
	void Scene::RenderQuad()
	{
		glBindVertexArray(planeVAO);
	    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void  Scene::RenderCube()
	{
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	void Scene::render(const OGLCamera &camera)
	{		
		// Lights
		glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
		glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 proj = glm::perspective(45.0f, 1.5f, 0.1f, 1000.0f);

		//////////////////////////////////////////////////////////////////////////////////
		// 1. Geometry Pass: render scene's geometry/normal/color data into gbuffer
		glUseProgram(model_prog);
		glBindFramebuffer(GL_FRAMEBUFFER, m_gbufferFbo);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(glGetUniformLocation(model_prog, "g_proj"), 1, GL_FALSE, &proj[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(model_prog, "g_view"), 1, GL_FALSE, &view[0][0]);

		// Floor cube
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
		glUniformMatrix4fv(glGetUniformLocation(model_prog, "g_model"), 1, GL_FALSE, &model[0][0]);
		RenderCube();

		// draw model on the floor
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0));
		model = glm::rotate(model, -90.0f, glm::vec3(1.0, 0.0, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		glUniformMatrix4fv(glGetUniformLocation(model_prog, "g_model"), 1, GL_FALSE,  &model[0][0]);
	    m_model.draw(model_prog);
	
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);

		/////////////////////////////2. Create SSAO texture//////////////////////////////////////
		glUseProgram(ssao_prog);

		glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFbo);
		glClear(GL_COLOR_BUFFER_BIT);

		glUniformMatrix4fv(glGetUniformLocation(ssao_prog, "g_proj"), 1, GL_FALSE, &proj[0][0]);
		glUniform1i(glGetUniformLocation(ssao_prog, "u_posDepthTex"), 0);
		glUniform1i(glGetUniformLocation(ssao_prog, "u_normalTex"), 1);
		glUniform1i(glGetUniformLocation(ssao_prog, "u_noiseTex"), 2);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_posDepthTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_normalTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_noiseTex);


		// Sample kernel, generates random floats between 0.0 and 1.0
		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
		std::default_random_engine generator;
		std::vector<glm::vec3> ssaoKernel;
		for (GLuint i = 0; i < 64; ++i) {
			glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			GLfloat scale = GLfloat(i) / 64.0;

			// Scale samples s.t. they're more aligned to center of kernel
			scale = lerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}

		// Send kernel + rotation 
		for (GLuint i = 0; i < 64; ++i) {
			glUniform3fv(glGetUniformLocation(ssao_prog, ("samples[" + std::to_string(i) + "]").c_str()), 1, &ssaoKernel[i][0]);
		}

		RenderQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glUseProgram(0);

		//////////////////////////////////////////////////////////////////////////////////////
		// 3. Blur SSAO texture to remove noise
		glUseProgram(blur_prog);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurFbo);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ssaoColorTex);
		RenderQuad();

		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		////////////////////////////////////////////////////////////////////////////////////////////
		// 4. Lighting Pass: traditional deferred Blinn-Phong lighting now with added screen-space ambient occlusion
		glUseProgram(light_prog);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_posDepthTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_normalTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_colorTex);
		glActiveTexture(GL_TEXTURE3); // Add extra SSAO texture to lighting pass
		glBindTexture(GL_TEXTURE_2D, m_ssaoColorBlurTex);

		glUniform1i(glGetUniformLocation(light_prog, "u_posDepthTex"), 0);
		glUniform1i(glGetUniformLocation(light_prog, "u_normalTex"), 1);
		glUniform1i(glGetUniformLocation(light_prog, "u_colorTex"), 2);
		glUniform1i(glGetUniformLocation(light_prog, "u_ssaoTex"), 3);

		// Also send light relevant uniforms
		glm::vec3 lightPosView = glm::vec3(camera.GetViewMatrix() * glm::vec4(lightPos, 1.0));
		glUniform3fv(glGetUniformLocation(light_prog, "light.Position"), 1, &lightPosView[0]);
		glUniform3fv(glGetUniformLocation(light_prog, "light.Color"), 1, &lightColor[0]);
		
		int draw_mode = 1;
		// Update attenuation parameters
		const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
		const GLfloat linear = 0.09;
		const GLfloat quadratic = 0.032;
		glUniform1f(glGetUniformLocation(light_prog, "light.Linear"), linear);
		glUniform1f(glGetUniformLocation(light_prog, "light.Quadratic"), quadratic);
		glUniform1i(glGetUniformLocation(light_prog, "draw_mode"), draw_mode);
		RenderQuad();

	}

	void Scene::shutdown()
	{
	}

	void Scene::init_buffer()
	{
	    m_model.loadModel("armadillo.obj");

		glGenBuffers(1, &planeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &cubeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Scene::init_vertexArray()
	{
		glGenVertexArrays(1, &planeVAO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glBindVertexArray(0);

		glGenVertexArrays(1, &cubeVAO);
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glBindVertexArray(0);
	}

	void Scene::init_shader()
	{
		modelShader.init();
		modelShader.attach(GL_VERTEX_SHADER,   "model.vert");
		modelShader.attach(GL_FRAGMENT_SHADER, "model.frag");
		modelShader.link();
		modelShader.info();
		model_prog =  modelShader.getProgram();

		lightShader.init();
		lightShader.attach(GL_VERTEX_SHADER,   "light.vert");
		lightShader.attach(GL_FRAGMENT_SHADER, "light.frag");
		lightShader.link();
		lightShader.info();
		light_prog =  lightShader.getProgram();

		blurShader.init();
		blurShader.attach(GL_VERTEX_SHADER,   "blur.vert");
		blurShader.attach(GL_FRAGMENT_SHADER, "blur.frag");
		blurShader.link();
		blurShader.info();
		blur_prog =  blurShader.getProgram();

		ssaoShader.init();
		ssaoShader.attach(GL_VERTEX_SHADER,   "ssao.vert");
		ssaoShader.attach(GL_FRAGMENT_SHADER, "ssao.frag");
		ssaoShader.link();
		ssaoShader.info();
		ssao_prog =  ssaoShader.getProgram();

	}

	void Scene::init_texture()
	{
	}

	void Scene::init_fbo()
	{
		float sw = WindowInfo::getInstance()->getWidth();
		float sh = WindowInfo::getInstance()->getHeight();

		// Set up G-Buffer, Three textures:
		// 1. Positions + depth (RGBA)
		// 2. Color (RGB) 
		// 3. Normals (RGB) 
		glGenFramebuffers(1, &m_gbufferFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_gbufferFbo);

		// - Position + linear depth color buffer
		glGenTextures(1, &m_posDepthTex);
		glBindTexture(GL_TEXTURE_2D, m_posDepthTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, sw, sh, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_posDepthTex, 0);
	

		// - Normal buffer texture
		glGenTextures(1, &m_normalTex);
		glBindTexture(GL_TEXTURE_2D, m_normalTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sw, sh, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalTex, 0);


		// - Albedo color buffer
		glGenTextures(1, &m_colorTex);
		glBindTexture(GL_TEXTURE_2D, m_colorTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sw, sh, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_colorTex, 0);

		// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);
	
		// - Create and attach depth buffer (renderbuffer)
		glGenRenderbuffers(1, &m_depthRbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthRbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, sw, sh);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRbo);

		// - Finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "GBuffer Framebuffer not complete!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		glGenFramebuffers(1, &m_ssaoFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFbo);
	
		// - SSAO color buffer
		glGenTextures(1, &m_ssaoColorTex);
		glBindTexture(GL_TEXTURE_2D, m_ssaoColorTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, sw, sh, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoColorTex, 0);
	
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "SSAO Framebuffer not complete!" << std::endl;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		///////////////////////////////////////////////////////////////////////////////////

		// - and blur stage
		glGenFramebuffers(1, &m_ssaoBlurFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurFbo);
		glGenTextures(1, &m_ssaoColorBlurTex);
		glBindTexture(GL_TEXTURE_2D, m_ssaoColorBlurTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, sw, sh, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoColorBlurTex, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/////////////////////////////////////////////////////////////////////////////////////////////

		// Sample kernel, generates random floats between 0.0 and 1.0
		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); 
		std::default_random_engine generator;

		// Noise texture, rotate around z-axis (in tangent space)
		std::vector<glm::vec3> ssaoNoise;
		for (GLuint i = 0; i < 16; i++) {
			glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); 
			ssaoNoise.push_back(noise);
		}

		glGenTextures(1, &m_noiseTex);
		glBindTexture(GL_TEXTURE_2D, m_noiseTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}


}