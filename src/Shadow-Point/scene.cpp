#include "Scene.h"

#include "ogl/loadTexture.h"
#include "ogl/Debug.h"



namespace byhj
{
	static ogl::OGLDebug oglDebug;
#pragma region VertexData


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

	void Scene::Init()
	{
		init_buffer();
		init_vertexArray();
		init_shader();
		init_texture();
		oglDebug.Debug();
	}


	void  Scene::RenderCube()
	{
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	void Scene::RenderScene(GLuint program)
	{
		// Room cube
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(10.0));
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
		glDisable(GL_CULL_FACE); // Note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
		glUniform1i(glGetUniformLocation(program, "reverse_normals"), 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
		RenderCube();
		glUniform1i(glGetUniformLocation(program, "reverse_normals"), 0); // And of course disable it
		glEnable(GL_CULL_FACE);
	
		// Cubes
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
		RenderCube();
		
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
		model = glm::scale(model, glm::vec3(1.5));
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
		RenderCube();
		
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
		RenderCube();
		
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
		RenderCube();
		
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
		model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(1.5));
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
		RenderCube();
	}

	void Scene::Render(const ogl::MvpMatrix &matrix, const ogl::Camera &camera, float aspect)
	{		
		
		glm::mat4 model = matrix.model;
	    glm::mat4 view  = matrix.view;
	    glm::mat4 proj  = matrix.proj;

		glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
		// Move light position over time
		//lightPos.z = sin(glfwGetTime() * 0.5) * 3.0;
		glm::vec3 camPos = camera.GetPos();

		GLfloat near_plane = 1.0f;
		GLfloat far_plane = 25.0f;
		glm::mat4 shadowProj = glm::perspective(90.0f, aspect, near_plane, far_plane);
		
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0)));
		
		// 1. Render scene to depth cubemap
		glUseProgram(shadow_prog);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		for (GLuint i = 0; i < 6; ++i)
		    glUniformMatrix4fv(glGetUniformLocation(shadow_prog, ("shadowTransforms[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, &( shadowTransforms[i] )[0][0] );
		
		glUniform1f(glGetUniformLocation( shadow_prog, "far_plane"), far_plane);
		glUniform3fv(glGetUniformLocation(shadow_prog, "lightPos"), 1, &lightPos[0]);
		RenderScene(shadow_prog);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glUseProgram(0);

		//////////////////////////////////////////////////////////////////////////////////////
		glUseProgram(scene_prog);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(glGetUniformLocation(scene_prog, "view"),  1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(scene_prog, "proj"),  1, GL_FALSE, &proj[0][0]);
		glUniform3fv(glGetUniformLocation(scene_prog, "lightPos"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(scene_prog, "viewPos"), 1, &camPos[0]);

		// Enable/Disable shadows by pressing 'SPscene_progACE'
		glUniform1i(glGetUniformLocation(scene_prog, "shadows"), shadowFlag);
		glUniform1f(glGetUniformLocation(scene_prog, "far_plane"), far_plane);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTex);
		glUniform1i(glGetUniformLocation(scene_prog, "diffuseTexture"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		glUniform1i(glGetUniformLocation(scene_prog, "depthMap"), 1);

		RenderScene(scene_prog);

		glUseProgram(0);
	

	}

	void Scene::Shutdown()
	{
	}

	void Scene::init_buffer()
	{
		glGenBuffers(1, &cubeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Scene::init_vertexArray()
	{

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
		SceneShader.init();
		SceneShader.attach(GL_VERTEX_SHADER,   "scene.vert");
		SceneShader.attach(GL_FRAGMENT_SHADER, "scene.frag");
		SceneShader.link();
		SceneShader.info();
		scene_prog = SceneShader.GetProgram();

		ShadowShader.init();
		ShadowShader.attach(GL_VERTEX_SHADER,   "shadow.vert");
		ShadowShader.attach(GL_FRAGMENT_SHADER, "shadow.frag");
		ShadowShader.link();
		ShadowShader.info();
		shadow_prog = ShadowShader.GetProgram();
	}

	void Scene::init_texture()
	{
		woodTex = ogl::loadTexture("../../../media/textures/wood.png");
	}

	void Scene::init_fbo(int sw, int sh)
	{
		glGenFramebuffers(1, &depthMapFBO);
		glGenTextures(1, &depthCubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

		for (GLuint i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, sw, sw, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// Attach cubemap as depth map FBO's color buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	}

	void Scene::ChangeShadow()
	{
		shadowFlag = !shadowFlag;
	}

}