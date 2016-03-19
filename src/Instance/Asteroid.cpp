#include "Asteroid.h"

namespace byhj
{
	static const int ASTEROID_AMOUNT = 1000;

	void Asteroid::Init(int sw, int sh)
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		m_Aspect = static_cast<GLfloat>(sw) / sh;

		m_LightGui.v_init(sw, sh);
		m_RotationGui.v_init(sw, sh);

		init_buffer();
		init_shader();
	}

	void Asteroid::Update(const glm::mat4 &camMat)
	{

		glUseProgram(m_Program);

		m_LightGui.v_update();
		m_RotationGui.v_update();

	    GLfloat time = static_cast<GLfloat>( glfwGetTime() );

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -30.0f)) * m_RotationGui.getRotationMat()
			* glm::rotate(glm::mat4(1.0f), time / 10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 view  = camMat;
		glm::mat4 proj  = glm::perspective(45.0f, m_Aspect, 0.1f, 1000.0f);

		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniform_loc.proj, 1, GL_FALSE, &proj[0][0]);
		glUniform3fv(uniform_loc.ambient, 1, &m_LightGui.getLight(0)[0]);

		glUseProgram(0);
	}

	void Asteroid::Render()
	{
		glUseProgram(m_Program);
		glBindVertexArray(m_vao);
	

		ModelMgr::getInstance()->renderInstance(m_Program, ASTEROID_AMOUNT);

		glDisable(GL_DEPTH_TEST);

		m_RotationGui.v_render();
		m_LightGui.v_render();
		glEnable(GL_DEPTH_TEST);



		glUseProgram(0);
	}

	void Asteroid::Shutdown()
	{
	}

	void Asteroid::init_buffer()
	{
		ModelMgr::getInstance()->loadOGLModel("rock/rock.obj");

		glm::mat4* modelMatrices = new glm::mat4[ASTEROID_AMOUNT];
		srand(glfwGetTime()); // initialize random seed	
		GLfloat radius = 15.0f;
		GLfloat offset = 2.0f;

		for (GLuint i = 0; i < ASTEROID_AMOUNT; i++)
		{
			glm::mat4 model;
			// 1. Translation: Randomly displace along circle with radius 'radius' in range [-offset, offset]
			GLfloat angle = (GLfloat)i / (GLfloat)ASTEROID_AMOUNT * 360.0f;
			GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
			GLfloat x = sin(angle) * radius + displacement;
			displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
			GLfloat y = -2.5f + displacement * 0.4f; // Keep height of asteroid field smaller compared to width of x and z
			displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
			GLfloat z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. Scale: Scale between 0.05 and 0.25f
			GLfloat scale = (rand() % 20) / 100.0f + 0.05;
			model = glm::scale(model, glm::vec3(scale));

			// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
			GLfloat rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// 4. Now add to list of matrices
			modelMatrices[i] = model;
		}

		for (GLuint i = 0; i < m_Model.getMeshCount(); i++)
		{
			m_vao = m_Model.getMeshVAO(i);

			GLuint buffer;
			glBindVertexArray(m_vao);
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, ASTEROID_AMOUNT * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

			// Set attribute pointers for matrix (4 times vec4) for instance
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}
		delete[] modelMatrices;
	}


	void Asteroid::init_shader()
	{
		m_AsteroidShader.init();
		m_AsteroidShader.attach(GL_VERTEX_SHADER,   "asteroid.vert");
		m_AsteroidShader.attach(GL_FRAGMENT_SHADER, "asteroid.frag");
		m_AsteroidShader.link();
		m_AsteroidShader.info();

		m_Program = m_AsteroidShader.getProgram();
		uniform_loc.model = glGetUniformLocation(m_Program, "u_Model");
		uniform_loc.view  = glGetUniformLocation(m_Program, "u_View");
		uniform_loc.proj  = glGetUniformLocation(m_Program, "u_Proj");
		
	}
}
