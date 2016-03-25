#include "oglSphereCamera.h"
#include <iostream>

namespace byhj {
   std::shared_ptr<OGLSphereCamera> OGLSphereCamera::getInstance() 
   {
	   static std::shared_ptr<OGLSphereCamera> m_pInstance = std::make_shared<OGLSphereCamera>();
	   return m_pInstance;
   }

   void OGLSphereCamera::update()
   {
	   // Convert Spherical to Cartesian coordinates.
	   float x = m_radius * sinf(m_phi) * cosf(m_theta);
	   float z = m_radius * sinf(m_phi) * sinf(m_theta);
	   float y = m_radius * cosf(m_phi);

	   // Build the view matrix.
	   m_pos    = glm::vec3(x, y, z);
	   m_target = glm::vec3(0.0f);
	   m_up     = glm::vec3(0.0f, 1.0f, 0.0f);
       m_view = glm::lookAt(m_pos, m_target, m_up);
	   std::cout << x << " " << y << " " << z << std::endl;
   }

   // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
   void OGLSphereCamera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
   {
       px = xoffset;
	   py = yoffset;
   }

   void OGLSphereCamera::ProcessMouseButton(GLFWwindow* window, int button, int state)
   {
	   if (state == GLFW_PRESS)
	   {
		   if (button == GLFW_MOUSE_BUTTON_LEFT) {
			   // Make each pixel correspond to a quarter of a degree.
			   float dx = 0.25f*static_cast<float>(px - m_lastMousePos.x);
			   float dy = 0.25f*static_cast<float>(py - m_lastMousePos.y);

			   // Update angles based on input to orbit D3DSphereCamera around box.
			   m_theta += dx;
			   m_phi   += dy;

			   // Restrict the angle mPhi.
			   m_phi = glm::clamp(m_phi, 0.1f, glm::pi<float>() - 0.1f);
		   }
		   else if (GLFW_MOUSE_BUTTON_MIDDLE) {

		   } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			   // Make each pixel correspond to 0.005 unit in the scene.
			   float dx = 0.005f*static_cast<float>(px - m_lastMousePos.x);
			   float dy = 0.005f*static_cast<float>(py - m_lastMousePos.y);

			   // Update the D3DSphereCamera radius based on input.
			   m_radius += dx - dy;

			   // Restrict the radius.
			   m_radius = glm::clamp(m_radius, 3.0f, 15.0f);
		   }
	   }
	   else
	   {
		   
	   }
	   m_lastMousePos.x = px;
		m_lastMousePos.y = py;
   }
   // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
   void OGLSphereCamera::ProcessMouseScroll(GLfloat yoffset)
   {
	   this->Zoom -= yoffset;
	   if (this->Zoom <= 1.0f)
		   this->Zoom = 1.0f;
	   if (this->Zoom >= 179.0f)
		   this->Zoom = 179.0f;
   }
   void OGLSphereCamera::mouse_callback(GLFWwindow* Triangle, double xpos, double ypos)
   {
	   if (firstMouse)
	   {
		   lastX = xpos;
		   lastY = ypos;
		   firstMouse = false;
	   }

	   GLfloat xoffset = xpos - lastX;
	   GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	   lastX = xpos;
	   lastY = ypos;
	   ProcessMouseMovement(xoffset, yoffset);
   }


   void OGLSphereCamera::scroll_callback(GLFWwindow* Triangle, double xoffset, double yoffset)
   {
	   ProcessMouseScroll(yoffset);
   }


   void OGLSphereCamera::mouseButton_callback(GLFWwindow *window, int x, int y, int z)
   {
	   ProcessMouseButton(window, x, y);
   }
   glm::mat4 OGLSphereCamera::getViewMat() const
   {
	   return m_view;
   }

   void OGLSphereCamera::setRadius(int r)
   {
	   m_radius = r;
   }
}