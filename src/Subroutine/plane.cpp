#include "Plane.h"
#include <glfw/glfw3.h>

namespace byhj {

	void Plane::init()
	{
		init_shader();
	}

	void Plane::update()
	{

	}

	void Plane::render()
	{
		glUseProgram(m_program);

		int i = static_cast<int>(glfwGetTime());

		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subroutines[i & 1]);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glUseProgram(0);
	}

	void Plane::shutdown()
	{

	}

	void Plane::init_shader()
	{
		m_SubShader.init();
		m_SubShader.attach(GL_VERTEX_SHADER, "subroutine.vert");
		m_SubShader.attach(GL_FRAGMENT_SHADER, "subroutine.frag");
		m_SubShader.link();
		m_SubShader.info();
		m_program = m_SubShader.getProgram();

		subroutines[0] = glGetSubroutineIndex(m_program, GL_FRAGMENT_SHADER, "myFunc1");
		subroutines[1] = glGetSubroutineIndex(m_program, GL_FRAGMENT_SHADER, "myFunc2");
		sub_loc = glGetSubroutineUniformLocation(m_program, GL_FRAGMENT_SHADER, "subUniform");
	}
}