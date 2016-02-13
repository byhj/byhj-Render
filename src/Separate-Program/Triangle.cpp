#include  "Triangle.h"

namespace byhj
{
 ///////////////////Vertex Data////////////////////////////////
 
	static const GLfloat VertexData[] =
	{
		//Front
		-0.5f, -0.5f,  0.0f,
		0.5f, -0.5f,  0.0f,
		0.0f,  0.5f,  0.0f,
	};
	static const GLsizei VertexSize = sizeof(VertexData);

	static const GLuint IndexData[] =
	{
	   0, 1, 2,
	};
	static const GLsizei IndexSize = sizeof(IndexData);

      Triangle::Triangle()
     {
     }

     Triangle::~Triangle()
     {
     }

	void Triangle::init()
	{
	
		init_shader();
		init_buffer();
		init_vertexArray();
	}

	void Triangle::init_buffer()
	{
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Triangle::init_vertexArray()
	{
	    glGenVertexArrays(1, &m_vao);
	    glBindVertexArray(m_vao);
	    
	    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	    
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	    
	    glBindVertexArray(0);
	    glBindBuffer(GL_ARRAY_BUFFER, 0);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Triangle::update()
	{

	}

	void Triangle::render()
	{
		glBindVertexArray(m_vao);
		glBindProgramPipeline(m_pipe);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 proj = glm::perspective(45.0f, 1.2f, 1.0f, 1000.0f);
		glm::mat4 mvp = model * view * proj;
		glProgramUniformMatrix4fv(m_vert, mvp_loc, 1, GL_FALSE, &mvp[0][0]);

		const static GLfloat color[4] ={ 1.0f, 0.0f, 0.0f, 1.0f };
		glProgramUniform4fv(m_frag, color_loc, 1, color);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindProgramPipeline(0);
	}

	void  Triangle::shutdown()
	{

		glDeleteProgramPipelines(1, &m_pipe);
		glDeleteBuffers(1, &m_ibo);
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);

	}
	void Triangle::init_shader()
	{
		glGenProgramPipelines(1, &m_pipe);

		auto vertSource = byhj::textFileRead("triangle.vert");
		const char *ver = vertSource.c_str();
		m_vert = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &ver);
		glUseProgramStages(m_pipe, GL_VERTEX_SHADER, m_vert);
		mvp_loc = glGetUniformLocation(m_vert, "mvp");


		auto fragContext = textFileRead("triangle.frag");
		const char *frg = fragContext.c_str();
		m_frag = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &frg);
		glUseProgramStages(m_pipe, GL_FRAGMENT_SHADER_BIT, m_frag);
		color_loc = glGetUniformLocation(m_frag, "color");

	}

}