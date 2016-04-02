#include "Sphere.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw/glfw3.h>

namespace byhj
{

Sphere::Sphere()
{

}

Sphere::~Sphere()
{

}

void Sphere::init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Sphere::update()
{
}

void Sphere::render()
{
	//Use this shader and vao data to render
	glUseProgram(program);
	glBindVertexArray(vao);

	float currentTime = static_cast<float>( glfwGetTime());
	glm::mat4 view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj  = glm::perspective(45.0f, 1.5f, 0.1f, 1000.0f);
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), currentTime, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniform.model_loc, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(uniform.view_loc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(uniform.proj_loc, 1, GL_FALSE, &proj[0][0]);
 
	ModelMgr::getInstance()->render("sphere.obj", program);

	glBindVertexArray(0);
	glUseProgram(0);
}


void Sphere::shutdown()
{
	glDeleteProgram(program);
}

void Sphere::init_buffer()
{
    ModelMgr::getInstance()->loadOGLModel("sphere.obj");

}

void Sphere::init_vertexArray()
{

}

void Sphere::init_shader()
{
	CubeShader.init();
	CubeShader.attach(GL_VERTEX_SHADER, "Sphere.vert");
	CubeShader.attach(GL_FRAGMENT_SHADER, "Sphere.frag");
	CubeShader.link();
	program = CubeShader.getProgram();

	uniform.model_loc = glGetUniformLocation(program, "model");
	uniform.view_loc = glGetUniformLocation(program, "view");
	uniform.proj_loc = glGetUniformLocation(program, "proj");
}

}