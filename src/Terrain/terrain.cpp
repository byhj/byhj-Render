#include "Terrain.h"
#include <glfw/glfw3.h>
#include "windowInfo.h"
#include "textureMgr.h"

#include <iostream>
#include <vector> 

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ktx.h"


namespace byhj
{

	Terrain::Terrain()
{

}

Terrain::~Terrain()
{

}

void Terrain::init()
{
	init_shader();
	init_buffer();
	init_vertexArray();
}

void Terrain::render()
{

	float currentTime =  glfwGetTime();

	float t = (float)currentTime * 0.03f;
	float r = sinf(t * 5.37f) * 15.0f + 16.0f;
	float h = cosf(t * 4.79f) * 2.0f + 5.2f;
	auto aspect = WindowInfo::getInstance()->getAspect();
	glm::mat4 mv_matrix  = OGLEulerCamera::getInstance()->getViewMat();
	glm::mat4 proj_matrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);

	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, glm::value_ptr(proj_matrix));
	glUniformMatrix4fv(uniforms.mvp_matrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_displacement);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_color);

	glUniform1f(uniforms.dmap_depth, enable_displacement ? dmap_depth : 0.0f);
	glUniform1i(uniforms.enable_fog, enable_fog ? 1 : 0);
	if (wireframe)
	   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);


	glUseProgram(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void Terrain::shutdown()
{
	glDeleteProgram(program);
}


void Terrain::init_buffer()
{

}

void Terrain::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}


void Terrain::init_shader()
{
	dispmapShader.init();
	dispmapShader.attach(GL_VERTEX_SHADER, "dispmap.vert");
	dispmapShader.attach(GL_TESS_CONTROL_SHADER, "dispmap.tcs");
	dispmapShader.attach(GL_TESS_EVALUATION_SHADER, "dispmap.tes");
	dispmapShader.attach(GL_FRAGMENT_SHADER, "dispmap.frag");
	dispmapShader.link();
	dispmapShader.info();
	program = dispmapShader.getProgram();

	uniforms.mv_matrix = glGetUniformLocation(program, "mv_matrix");
	uniforms.mvp_matrix = glGetUniformLocation(program, "mvp_matrix");
	uniforms.proj_matrix = glGetUniformLocation(program, "proj_matrix");
	uniforms.dmap_depth = glGetUniformLocation(program, "dmap_depth");
	uniforms.enable_fog = glGetUniformLocation(program, "enable_fog");
	dmap_depth = 6.0f;

	tex_displacement = loadKtx("../../media/textures/terragen1.ktx");
	tex_color = loadKtx("../../media/textures/terragen_color.ktx");
}

}