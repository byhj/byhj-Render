#include "Clip.h"
#include "windowInfo.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glfw/glfw3.h>

namespace byhj
{

Clip::Clip()
{

}

Clip::~Clip()
{

}

void Clip::init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_CLIP_DISTANCE1);

	init_buffer();
	init_vertexArray();
	init_shader();
}

void Clip::render()
{

	float f = (float)glfwGetTime();

	glUseProgram(m_program);
	float currTime = static_cast<float>(glfwGetTime()) / 10.0f;
	static float aspect = WindowInfo::getInstance()->getAspect();
	glm::mat4 proj_matrix = glm::perspective(50.0f, aspect, 0.1f, 1000.0f);

	glm::mat4 mv_matrix = glm::rotate(glm::mat4(1.0f), currTime, glm::vec3(1.0f, 1.0f, 1.0f));

	glm::mat4 plane_matrix = glm::rotate(glm::mat4(1.0f), f * 6.0f, glm::vec3(1.0f, 0.0f, 0.0f) )
	                       * glm::rotate(glm::mat4(1.0f), f * 7.3f, glm::vec3(0.0f, 1.0f, 0.0f) );

	glm::vec4 plane = plane_matrix[0];
	plane[3] = 0.0f;
	plane = glm::normalize(plane);
	glm::vec4 clip_sphere = glm::vec4(sinf(f * 0.7f) * 3.0f, cosf(f * 1.9f) * 3.0f,
		sinf(f * 0.1f) * 3.0f, cosf(f * 1.7f) + 2.5f);


	m_model.draw(m_program);

	glUseProgram(0);
}


void Clip::shutdown()
{
	glDeleteProgram(m_program);
}

void Clip::init_buffer()
{
	m_model.loadModel();
}

void Clip::init_vertexArray()
{

}

void Clip::init_shader()
{
	m_clipShader.init();
	m_clipShader.attach(GL_VERTEX_SHADER, "Clip.vert");
	m_clipShader.attach(GL_FRAGMENT_SHADER, "Clip.frag");
	m_clipShader.link();
	m_clipShader.info();
	m_program = m_clipShader.getProgram();

	glUseProgram(m_program);
 
}

}