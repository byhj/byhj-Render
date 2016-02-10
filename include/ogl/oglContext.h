#ifndef OGLContext_H
#define OGLContext_H


// Include Windows functions
#include <Windows.h>

// Include input and output operations
#include <iostream>

// OpenGL and GLEW Header Files and Libraries
#include <GL/glew.h>
#include <GL/wglew.h>
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

namespace byhj {
	class OGLContext {
	public:
		OGLContext();
		OGLContext(HWND hwnd);
		~OGLContext();

		void createContext(HWND hwnd);
		void render();

	private:
		int sw, sh;
	private:
		HGLRC hrc; // Rendering context
		HDC hdc; // Device context
		HWND hwnd; // Window identifier
	};
}
#endif