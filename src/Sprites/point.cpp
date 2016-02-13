#include  "Point.h"
#include <glfw/glfw3.h>
#include "textureMgr.h"

namespace byhj
{
 ///////////////////Vertex Data////////////////////////////////
 
	const int NUM_STARS = 2000;

	struct Star {
		glm::vec3 pos;
		glm::vec3 color;
	};

      Point::Point()
     {
     }

     Point::~Point()
     {
     }

	void Point::init()
	{
	
		init_shader();
		init_buffer();
		init_vertexArray();
		TextureMgr::getInstance()->loadOGLTexture("star.bmp");
	}

	void Point::init_buffer()
	{
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, NUM_STARS * sizeof(Star), NULL, GL_STATIC_DRAW);

		Star *pStar = (Star *)glMapBufferRange(GL_ARRAY_BUFFER, 0, NUM_STARS * sizeof(Star), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		for (int i = 0; i != NUM_STARS; ++i)
		{
			pStar[i].pos = glm::vec3((random_float() * 2.0f - 1.0f) * 100.0f, (random_float() * 2.0f - 1.0f) * 100.0f, random_float());
			pStar[i].color = glm::vec3(0.8f + random_float() * 0.2f, random_float() * 0.5, random_float());
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	}

	void Point::init_vertexArray()
	{
	    glGenVertexArrays(1, &m_vao);
	    glBindVertexArray(m_vao);
	    
	    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	    glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Star), 0);
	    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Star), GL_BUFFER_OFFSET(sizeof(glm::vec3)));
	   
	    glBindVertexArray(0);
	    glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Point::update()
	{
		glUseProgram(m_program);

		auto time = glfwGetTime();
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)time, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 proj  = glm::perspective(45.0f, 1.5f, 0.1f, 1000.0f);

		glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(u_view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(u_proj, 1, GL_FALSE, &proj[0][0]);
	
		float t = glfwGetTime();
		t *= 0.1f;
		t -= floor(t);
		std::cout << t << std::endl;
		glUniform1f(time_loc, t);
		GLuint texId = TextureMgr::getInstance()->getOGLTextureByName("star.bmp");
		glBindTexture(GL_TEXTURE_2D, texId);

		glUseProgram(0);
	}

	void Point::render()
	{
		glBindVertexArray(m_vao);
		glUseProgram(m_program);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_POINT_SPRITE);

		glDrawArrays(GL_POINTS, 0, NUM_STARS);

		glBindVertexArray(0);
		glUseProgram(0);
	}

	void  Point::shutdown()
	{

		glDeleteProgram(m_program);
		glDeleteBuffers(1, &m_ibo);
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);

	}
	void Point::init_shader()
	{
	    m_PointShader.init();
	    m_PointShader.attach(GL_VERTEX_SHADER, "Point.vert");
	    m_PointShader.attach(GL_FRAGMENT_SHADER, "Point.frag");
	    m_PointShader.link();
	    m_program = m_PointShader.getProgram();

		u_model = glGetUniformLocation(m_program, "g_model");
		u_view  = glGetUniformLocation(m_program, "g_view");
		u_proj  = glGetUniformLocation(m_program, "g_proj");
		time_loc = glGetUniformLocation(m_program, "time");

	}

}