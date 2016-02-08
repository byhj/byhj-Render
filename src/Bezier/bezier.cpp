#include "bezier.h"
#include <glfw/glfw3.h>

namespace byhj {

	static const GLfloat VertexData[] =
	{
		-1.0f,  -1.0f,  0.0f,
		-0.33f, -1.0f,  0.0f,
		0.33f, -1.0f,  0.0f,
		1.0f,  -1.0f,  0.0f,

		-1.0f,  -0.33f, 0.0f,
		-0.33f, -0.33f, 0.0f,
		0.33f, -0.33f, 0.0f,
		1.0f,  -0.33f, 0.0f,

		-1.0f,   0.33f, 0.0f,
		-0.33f,  0.33f, 0.0f,
		0.33f,  0.33f, 0.0f,
		1.0f,   0.33f, 0.0f,

		-1.0f,   1.0f,  0.0f,
		-0.33f,  1.0f,  0.0f,
		0.33f,  1.0f,  0.0f,
		1.0f,   1.0f,  0.0f,
	};
	static const GLuint IndexData[] =
	{
		0, 1, 1, 2, 2, 3,
		4, 5, 5, 6, 6, 7,
		8, 9, 9, 10, 10, 11,
		12, 13, 13, 14, 14, 15,

		0, 4, 4, 8, 8, 12,
		1, 5, 5, 9, 9, 13,
		2, 6, 6, 10, 10, 14,
		3, 7, 7, 11, 11, 15
	};

	Bezier::Bezier()
	{

	}

	Bezier::~Bezier()
	{

	}

	void Bezier::init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
	}

	void Bezier::update()
	{

	}

	void Bezier::render()
	{
		float currentTime =  glfwGetTime();
		static double last_time = 0.0;
		static double total_time = 0.0;

		total_time += (currentTime - last_time);
		last_time = currentTime;
		float t = (float)total_time;

		//your should bind the buffer first
		glBindVertexArray(m_bezierVar.vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_bezierVar.vbo);

		glm::vec3 * p = (glm::vec3 *)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(patch_data),
			                                          GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		memcpy(p, VertexData, sizeof(VertexData));
		for (int i = 0; i < 16; i++)
		{
			float fi = (float)i / 16.0f;
			p[i][2] = sinf(t * (0.2f + fi * 0.3f));
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		////////////////////////////////////////////////////////////////////////////
		glUseProgram(m_bezierVar.program);
		glBindVertexArray(m_bezierVar.vao);

		glm::mat4 proj  = glm::perspective(50.0f, 1.0f, 1.0f, 1000.0f);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f) )
			* glm::rotate(glm::mat4(1.0f), glm::radians(t * 20.0f), glm::vec3(0.0f, 1.0f, 0.0f) )
			* glm::rotate(glm::mat4(1.0f), glm::radians(t * 34.0f), glm::vec3(1.0f, 0.0f, 0.0f) );
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(m_bezierVar.mv_loc, 1, GL_FALSE, &(view * model)[0][0]);
		glUniformMatrix4fv(m_bezierVar.proj_loc, 1, GL_FALSE, &proj[0][0]);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glPatchParameteri(GL_PATCH_VERTICES, 16);
		glDrawArrays(GL_PATCHES, 0, 16);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindVertexArray(0);
		glUseProgram(0);

		//////////////////////////////////////////////////////////////////////////////////////
		glUseProgram(m_cvVar.program);
		glBindVertexArray(m_bezierVar.vao);

		glm::mat4 mvp = proj * view * model;
		glUniformMatrix4fv(m_cvVar.mvp_loc, 1, GL_FALSE, &mvp[0][0]);

		glPointSize(9.0f);
		glUniform4fv(m_cvVar.color_loc, 1, &(glm::vec4(0.2f, 0.7f, 0.9f, 1.0f))[0]);
		glDrawArrays(GL_POINTS, 0, 16);

		glUniform4fv(m_cvVar.color_loc, 1, &(glm::vec4(0.7f, 0.9f, 0.2f, 1.0f))[0]);
		glDrawElements(GL_LINES, 48, GL_UNSIGNED_INT, NULL);
	
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void Bezier::shutdown()
	{

	}

	void Bezier::init_buffer()
	{
		glGenBuffers(1, &m_bezierVar.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_bezierVar.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_cvVar.ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cvVar.ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexData), IndexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Bezier::init_shader()
	{
		m_BezierShader.init();
		m_BezierShader.attach(GL_VERTEX_SHADER, "bezier.vert");
		m_BezierShader.attach(GL_TESS_CONTROL_SHADER, "bezier.tcs");
		m_BezierShader.attach(GL_TESS_EVALUATION_SHADER, "bezier.tes");
		m_BezierShader.attach(GL_FRAGMENT_SHADER, "bezier.frag");
		m_BezierShader.link();
		m_BezierShader.info();
		m_bezierVar.program = m_BezierShader.getProgram();
		m_bezierVar.mv_loc = glGetUniformLocation(m_bezierVar.program, "u_mv");
		m_bezierVar.proj_loc  = glGetUniformLocation(m_bezierVar.program, "u_proj");
		

		m_CVShader.init();
		m_CVShader.attach(GL_VERTEX_SHADER, "cv.vert");
		m_CVShader.attach(GL_FRAGMENT_SHADER, "cv.frag");
		m_CVShader.link();
		m_CVShader.info();
		m_cvVar.program = m_CVShader.getProgram();
		m_cvVar.color_loc = glGetUniformLocation(m_cvVar.program, "u_color");
		m_cvVar.mvp_loc   = glGetUniformLocation(m_cvVar.program, "u_mvp");
	}

	void Bezier::init_vertexArray()
	{
		glGenVertexArrays(1, &m_bezierVar.vao);
		glBindVertexArray(m_bezierVar.vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cvVar.ibo);

		glBindBuffer(GL_ARRAY_BUFFER, m_bezierVar.vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}