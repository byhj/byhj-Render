#include "OGLEulerCamera.h"

namespace byhj
{
	std::shared_ptr<OGLEulerCamera> OGLEulerCamera::m_pInstance = std::make_shared<OGLEulerCamera>();

	std::shared_ptr<OGLEulerCamera> OGLEulerCamera::getInstance() {

		return m_pInstance;
	}
	// Constructor with vectors
	OGLEulerCamera::OGLEulerCamera(glm::vec3 position , glm::vec3 up , GLfloat yaw , GLfloat pitch )
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVTY),
		Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->ctr = true;
		this->updateCameraVectors();

		firstMouse = true;
		deltaTime = 0.0f;

		for (int i = 0; i != 1024; ++i)
			keys[i] = false;
	}

	// Constructor with scalar values
	OGLEulerCamera::OGLEulerCamera(GLfloat posX, GLfloat posY, GLfloat posZ,
		GLfloat upX, GLfloat upY,
		GLfloat upZ, GLfloat yaw, GLfloat pitch)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVTY),
		Zoom(ZOOM)
	{
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	// Processes input received from any keyboard-like input system. 
	//Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void OGLEulerCamera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->MovementSpeed * deltaTime * 3.0f;
		if (direction == FORWARD)
			this->Position += this->Front * velocity;
		if (direction == BACKWARD)
			this->Position -= this->Front * velocity;
		if (direction == LEFT)
			this->Position -= this->Right * velocity;
		if (direction == RIGHT)
			this->Position += this->Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void OGLEulerCamera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
	{
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw   += xoffset;
		this->Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		if (this->ctr)
			this->updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void OGLEulerCamera::ProcessMouseScroll(GLfloat yoffset)
	{
		this->Zoom -= yoffset;
		if (this->Zoom <= 1.0f)
			this->Zoom = 1.0f;
		if (this->Zoom >= 179.0f)
			this->Zoom = 179.0f;
	}


	void OGLEulerCamera::updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
	}


	// Moves/alters the camera positions based on user input
	void OGLEulerCamera::movement(GLFWwindow *Triangle)
	{
		if (keys[GLFW_KEY_ESCAPE] )
			glfwSetWindowShouldClose(Triangle, GL_TRUE);

		// Camera controls
		if(keys[GLFW_KEY_W])
			ProcessKeyboard(FORWARD, deltaTime);
		if(keys[GLFW_KEY_S])
			ProcessKeyboard(BACKWARD, deltaTime);
		if(keys[GLFW_KEY_A])
			ProcessKeyboard(LEFT, deltaTime);
		if(keys[GLFW_KEY_D])
			ProcessKeyboard(RIGHT, deltaTime);
		if (keys[GLFW_KEY_C])
		{
			glfwSetInputMode(Triangle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			ctr = false;
		}
	}

	// Is called whenever a key is pressed/released via GLFW
	void OGLEulerCamera::key_callback(GLFWwindow* Triangle, int key, int scancode, int action, int mode)
	{
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(Triangle, GL_TRUE);

		if(action == GLFW_PRESS)
			keys[key] = true;
		else if(action == GLFW_RELEASE)
			keys[key] = false;	
	}

	void OGLEulerCamera::mouse_callback(GLFWwindow* Triangle, double xpos, double ypos)
	{
		if(firstMouse)
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


	void OGLEulerCamera::scroll_callback(GLFWwindow* Triangle, double xoffset, double yoffset)
	{
		ProcessMouseScroll(yoffset);
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 OGLEulerCamera::getViewMat() const
	{
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}
	float OGLEulerCamera::getZoom() const
	{
		return Zoom;
	}

	void OGLEulerCamera::update(GLfloat dt)
	{
		deltaTime = dt;
	}

	void OGLEulerCamera::setPos(const glm::vec3 &pos)
	{
		Position = pos;
	}

	glm::vec3 OGLEulerCamera::getPos() const
	{
		return Position;
	}
	glm::vec3 OGLEulerCamera::getFront() const 
	{
		return Front;
	}
}
