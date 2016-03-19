#include "TessTriangle.h"
#include "windowInfo.h"

namespace byhj
{
	static  const GLuint IndexData[] ={
		2, 1, 0,
		3, 2, 0,
		4, 3, 0,
		5, 4, 0,
		1, 5, 0,

		11, 6,  7,
		11, 7,  8,
		11, 8,  9,
		11, 9,  10,
		11, 10, 6,

		1, 2, 6,
		2, 3, 7,
		3, 4, 8,
		4, 5, 9,
		5, 1, 10,

		2,  7, 6,
		3,  8, 7,
		4,  9, 8,
		5, 10, 9,
		1, 6, 10
	};

	static const float VertexData[] ={
		0.000f,  0.000f,  1.000f,
		0.894f,  0.000f,  0.447f,
		0.276f,  0.851f,  0.447f,
		-0.724f,  0.526f,  0.447f,
		-0.724f, -0.526f,  0.447f,
		0.276f, -0.851f,  0.447f,
		0.724f,  0.526f, -0.447f,
		-0.276f,  0.851f, -0.447f,
		-0.894f,  0.000f, -0.447f,
		-0.276f, -0.851f, -0.447f,
		0.724f, -0.526f, -0.447f,
		0.000f,  0.000f, -1.000f
	};



	void TessTriangle::init()
	{


		m_TessGUI.v_init();
		init_buffer();
		init_vertexArray();
		init_shader();

	}

	void TessTriangle::update()
	{
		glUseProgram(program);

		auto tessInner  = m_TessGUI.getTessInnder();
		auto tessOuter  = m_TessGUI.getTessOuter();
		auto lightDir   = m_TessGUI.getLightDir();
		auto ambientMat = m_TessGUI.getAmbientMat();
		auto diffuseMat = m_TessGUI.getDiffuseMat();

		glUniform1i(uniform_loc.tessInner, tessInner);
		glUniform1i(uniform_loc.tessOuter, tessOuter);
		glUniform3fv(uniform_loc.lightDir, 1, &lightDir[0]);
		glUniform4fv(uniform_loc.ambient, 1, &ambientMat[0]);
		glUniform4fv(uniform_loc.diffuse, 1, &diffuseMat[0]);

		float elapsedTime  = glfwGetTime() / 5.0f;
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), elapsedTime, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 proj = glm::perspective(45.0f, WindowInfo::getInstance()->getAspect(), 0.1f, 1000.0f);

		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniform_loc.proj, 1, GL_FALSE, &proj[0][0]);

		glUseProgram(0);
	}

	void TessTriangle::render()
	{

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		auto wireFrame  = m_TessGUI.getWireFrame();
		glPolygonMode(GL_FRONT_AND_BACK, wireFrame ? GL_LINE : GL_FILL);

		glUseProgram(program);
		glBindVertexArray(vao);

		//Render the scene
		glPatchParameteri(GL_PATCH_VERTICES, 3);
		glDrawElements(GL_PATCHES, sizeof(IndexData) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);

		//Render the GUI
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		m_TessGUI.v_render();
	}

	void TessTriangle::shutdown()
	{
		m_TessGUI.v_shutdown();

		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
	}

	void TessTriangle::init_buffer()
	{

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexData), IndexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	void TessTriangle::init_vertexArray()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}
	void TessTriangle::init_shader()
	{
		TessShader.init();
		TessShader.attach(GL_VERTEX_SHADER, "triangle.vert");
		TessShader.attach(GL_TESS_CONTROL_SHADER, "triangle.tcs");
		TessShader.attach(GL_TESS_EVALUATION_SHADER, "triangle.tes");
		TessShader.attach(GL_GEOMETRY_SHADER, "triangle.geom");
		TessShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
		TessShader.link();
		TessShader.info();
		program = TessShader.getProgram();

		uniform_loc.model = glGetUniformLocation(program, "model");
		uniform_loc.view  = glGetUniformLocation(program, "view");
		uniform_loc.proj  = glGetUniformLocation(program, "proj");


		uniform_loc.ambient  = glGetUniformLocation(program, "ambientMat");
		uniform_loc.diffuse  = glGetUniformLocation(program, "diffuseMat");
		uniform_loc.lightDir = glGetUniformLocation(program, "lightDir");

		uniform_loc.tessInner = glGetUniformLocation(program, "tessInner");
		uniform_loc.tessOuter = glGetUniformLocation(program, "tessOuter");

	}


}