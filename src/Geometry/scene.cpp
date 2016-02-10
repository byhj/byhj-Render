#include "scene.h"

namespace byhj {
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::init()
	{
		init_buffer();
		init_vertexArray();
		init_shader();
	}

	void Scene::update()
	{

	}

	void Scene::render()
	{
		glUseProgram(m_program);
		glBindVertexArray(m_vao);

		glDrawElements(GL_TRIANGLES, m_mesh.IndexData.size(), GL_UNSIGNED_INT, 0);

		glUseProgram(0);
		glBindVertexArray(0);
	}

	void Scene::shutdown()
	{

	}

	void Scene::init_buffer()
	{
	
		Geometry::getInstance()->createCube(1.0f, 1.0f, 1.0f, m_mesh);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_mesh.VertexData.size() * sizeof(OGLVertex)), &m_mesh.VertexData[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_mesh.IndexData.size() * sizeof(GLuint) ), &m_mesh.IndexData[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Scene::init_vertexArray()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OGLVertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(OGLVertex), GL_BUFFER_OFFSET(sizeof(glm::vec3)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OGLVertex), GL_BUFFER_OFFSET(sizeof(glm::vec3) * 2));

		glBindVertexArray(0);

	}

	void Scene::init_shader()
	{
		m_SceneShader.init();
		m_SceneShader.attach(GL_VERTEX_SHADER, "scene.vert");
		m_SceneShader.attach(GL_FRAGMENT_SHADER, "scene.frag");
		m_SceneShader.link();
		m_SceneShader.info();
		m_program = m_SceneShader.getProgram();

	}
}