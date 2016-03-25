#include "MeshLoad.h"
#include "textureMgr.h"
#include "modelMgr.h"
#include "windowInfo.h"

namespace byhj {
  MeshLoad::MeshLoad()
  {
  }

  MeshLoad::~MeshLoad()
  {

  }
  
  void MeshLoad::init()
  {
	  init_buffer();
	  init_shader();
  }

  void MeshLoad::update()
  {

  }

  void MeshLoad::render()
  {
	  glUseProgram(m_program);


	  glActiveTexture(GL_TEXTURE0);
	  GLuint skybox_tex = TextureMgr::getInstance()->getOGLTextureByName("sea");
	  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_tex);

	  glm::mat4 model = glm::mat4(1.0f);
	  glm::mat4 view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -3.0f),
	                                glm::vec3(0.0f, 1.0f, 0.0f));
	  glm::mat4 proj  = glm::perspective(45.0f, WindowInfo::getInstance()->getAspect(), 0.1f, 1000.0f);

	  glUniformMatrix4fv(uniformLoc.model, 1, GL_FALSE, &model[0][0]);
	  glUniformMatrix4fv(uniformLoc.view, 1, GL_FALSE, &view[0][0]);
	  glUniformMatrix4fv(uniformLoc.proj, 1, GL_FALSE, &proj[0][0]);

	  ModelMgr::getInstance()->render(m_program);

	  glUseProgram(0);
  }

  void MeshLoad::Shutdown()
  {

  }

  void MeshLoad::init_buffer()
  {
	  ModelMgr::getInstance()->loadOGLModel("dragon.obj");

	  std::vector<std::string> faces;
	  faces.push_back("/skyboxs/sea/right.jpg");
	  faces.push_back("/skyboxs/sea/left.jpg");
	  faces.push_back("/skyboxs/sea/top.jpg");
	  faces.push_back("/skyboxs/sea/bottom.jpg");
	  faces.push_back("/skyboxs/sea/back.jpg");
	  faces.push_back("/skyboxs/sea/front.jpg");

	  skyboxTex = TextureMgr::getInstance()->loadOGLTexture("sea", faces);
  }

  void MeshLoad::init_shader()
  {
	  m_meshShader.init();
	  m_meshShader.attach(GL_VERTEX_SHADER, "model.vert");
	  m_meshShader.attach(GL_FRAGMENT_SHADER, "model.frag");
	  m_meshShader.link();
	  m_meshShader.info();

	  uniformLoc.model = glGetUniformLocation(m_program, "u_model");
	  uniformLoc.view  = glGetUniformLocation(m_program, "u_view");
	  uniformLoc.proj  = glGetUniformLocation(m_program, "u_proj");
	  uniformLoc.skybox = glGetUniformLocation(m_program, "u_skybox");
  }
}