#include "oglContext.h"

namespace byhj {

	OGLContext::OGLContext()
	{

	}

	OGLContext::OGLContext(HWND hwnd)
	{
		createContext(hwnd);
	}

	OGLContext::~OGLContext()
	{
		wglMakeCurrent(hdc, 0); // Remove the rendering context from our device context
		wglDeleteContext(hrc); // Delete our rendering context

		ReleaseDC(hwnd, hdc); // Release the device context from our window
	}

	void OGLContext::createContext(HWND hwnd)
	{
		this->hwnd = hwnd;

		hdc = GetDC(hwnd); // Get the device context for our window

		PIXELFORMATDESCRIPTOR pfd; // Create a new PIXELFORMATDESCRIPTOR (PFD)
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window
		pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels
		pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)
		pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)
		pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD

		int nPixelFormat = ChoosePixelFormat(hdc, &pfd); // Check if our PFD is valid and get a pixel format back

		bool bResult = SetPixelFormat(hdc, nPixelFormat, &pfd); // Try and set the pixel format based on our PFD

		HGLRC tempOpenGLContext = wglCreateContext(hdc); // Create an OpenGL 2.1 context for our device context
		wglMakeCurrent(hdc, tempOpenGLContext); // Make the OpenGL 2.1 context current and active
		GLenum error = glewInit(); // Enable GLEW

		int attributes[] ={
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4, // Set the MAJOR version of OpenGL to 4
			WGL_CONTEXT_MINOR_VERSION_ARB, 3, // Set the MINOR version of OpenGL to 3
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
			0
		};

		if (wglewIsSupported("WGL_ARB_create_context") == 1) { // If the OpenGL 3.x context creation extension is available
			hrc = wglCreateContextAttribsARB(hdc, NULL, attributes); // Create and OpenGL 3.x context based on the given attributes
			wglMakeCurrent(NULL, NULL); // Remove the temporary context from being active
			wglDeleteContext(tempOpenGLContext); // Delete the temporary OpenGL 2.1 context
			wglMakeCurrent(hdc, hrc); // Make our OpenGL 3.0 context current
		}
		else {
			hrc = tempOpenGLContext; // If we didn't have support for OpenGL 3.x and up, use the OpenGL 2.1 context
		}

		int glVersion[2] ={ -1, -1 }; // Set some default values for the version
		glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // Get back the OpenGL MAJOR version we are using
		glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // Get back the OpenGL MAJOR version we are using

		std::cout << "Using OpenGL: " << glVersion[0] << "." << glVersion[1] << std::endl; // Output which version of OpenGL we are using
	}

	void OGLContext::render()
	{
		SwapBuffers(hdc); // Swap buffers so we can see our rendering
	}
}