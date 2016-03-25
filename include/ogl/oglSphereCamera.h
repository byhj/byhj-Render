#ifndef OGLSphereCamera_H
#define OGLSphereCamera_H

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include <glfw/glfw3.h>

namespace byhj {
    class OGLSphereCamera {
	public:
		OGLSphereCamera() = default;
		~OGLSphereCamera() = default;

		void update();

		glm::mat4 getViewMat() const;
		void setRadius(int r);
		glm::vec3  getCamPos() const;
		glm::vec3  getCamTarget() const;
		glm::vec3  getCamUp() const;

		static std::shared_ptr<OGLSphereCamera> getInstance() ;

		void mouse_callback(GLFWwindow* Triangle, double xpos, double ypos);
		void scroll_callback(GLFWwindow* Triangle, double xoffset, double yoffset);
		void mouseButton_callback(GLFWwindow* window, int button, int state, int mod);
	private:
		void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
		void ProcessMouseScroll(GLfloat yoffset);
		void ProcessMouseButton(GLFWwindow* window, int button, int state);

		float m_radius = 5.0f;
		float m_theta = 1.5f * glm::pi<float>();
		float m_phi = 0.25f  * glm::pi<float>();

		// Camera options
		GLfloat MovementSpeed;
		GLfloat MouseSensitivity;
		GLfloat Zoom;
		bool ctr;
		bool keys[1024];
		GLfloat lastX, lastY;
		bool firstMouse;
		GLfloat deltaTime;
		glm::vec3 m_pos;
		glm::vec3 m_target;
		glm::vec3 m_up;
		glm::mat4 m_view;
		glm::vec3 m_lastMousePos;
		float px, py;
	};
}
#endif