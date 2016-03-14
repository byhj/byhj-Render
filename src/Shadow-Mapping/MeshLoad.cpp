



#include "MeshLoad.h"
#include "windowInfo.h"

namespace byhj
{
	GLfloat planeVertices[] ={
		// Positions          // Normals         // Texture Coords
		 25.0f, 0.0f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
		-25.0f, 0.0f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
		-25.0f, 0.0f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				
		 25.0f, 0.0f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
		 25.0f, 0.0f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
		-25.0f, 0.0f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f
	};

	static  const GLuint IndexData[] ={
		0, 1, 2, 2, 3, 0
	};
	static const GLsizei IndexSize = sizeof(IndexData);

	void MeshLoad::Init()
	{
		init_buffer();
		init_shader();
		init_fbo();
	}

	void MeshLoad::Update()
	{

	}

	void MeshLoad::Render(const OGLCamera &camera)
	{

		glm::mat4 model = glm::rotate(glm::mat4(1.0f), 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 view  = camera.GetViewMatrix();
		glm::mat4 proj  = glm::perspective(45.0f, 1.5f, 0.1f, 1000.0f);

		GLfloat near_plane = 0.1f, far_plane = 100.0f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::vec3 lightPos(0.0f, 5.0f, 3.0f);
		glm::vec3 camPos = camera.GetPos();

		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;


		glUseProgram(shadowProgram);

		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view, 1, GL_FALSE, &lightSpaceMatrix[0][0]);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		m_Model.draw(shadowProgram);
		glBindVertexArray(planeVAO);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glBindVertexArray(0);
		glUseProgram(0);
		
		/////////////////////////////////////////////////////////////////////////////

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glUseProgram(lightProgram);

		glUniformMatrix4fv(glGetUniformLocation(lightProgram, "model"), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(lightProgram, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(lightProgram, "proj"), 1, GL_FALSE, &proj[0][0]);
		glUniform3fv(glGetUniformLocation(lightProgram, "lightPos"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(lightProgram, "viewPos"), 1, &camPos[0]);
		glUniformMatrix4fv(glGetUniformLocation(lightProgram, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, benchTex);
		glUniform1i(texLocs[0], 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTex);

		glUniform1i(texLocs[1], 1);

		m_Model.draw(lightProgram);
		glBindVertexArray(planeVAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planeTex);
		glUniform1i(texLocs[0], 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		glUseProgram(0);
	}

	void MeshLoad::Shutdown()
	{
	}

	void MeshLoad::init_buffer()
	{
		glGenBuffers(1, &planeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &planeVAO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glBindVertexArray(0);

		m_Model.loadModel("bench.obj");
	}


	void MeshLoad::init_shader()
	{
		m_MeshLoadShader.init();
		m_MeshLoadShader.attach(GL_VERTEX_SHADER, "shadow.vert");
		m_MeshLoadShader.attach(GL_FRAGMENT_SHADER, "shadow.frag");
		m_MeshLoadShader.link();
		m_MeshLoadShader.info();
		shadowProgram = m_MeshLoadShader.getProgram();
		uniform_loc.model = glGetUniformLocation(shadowProgram, "u_model");
		uniform_loc.view  = glGetUniformLocation(shadowProgram, "u_lightMat");


		PlaneShader.init();
		PlaneShader.attach(GL_VERTEX_SHADER, "plane.vert");
		PlaneShader.attach(GL_FRAGMENT_SHADER, "plane.frag");
		PlaneShader.link();
		PlaneShader.info();
		lightProgram = PlaneShader.getProgram();
		texLocs[0] = glGetUniformLocation( lightProgram, "diffuseTexture");
		texLocs[1] = glGetUniformLocation(lightProgram, "shadowMap");
		planeTex = TextureMgr::getInstance()->loadOGLTexture("wood.png");
		benchTex = TextureMgr::getInstance()->loadOGLTexture("wood.png");
	}

	void MeshLoad::init_fbo()
	{
		auto sw = WindowInfo::getInstance()->getWidth();
		auto sh = WindowInfo::getInstance()->getHeight();
		glGenTextures(1, &depthTex);
		glBindTexture(GL_TEXTURE_2D, depthTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, sw, sh, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[] ={ 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
