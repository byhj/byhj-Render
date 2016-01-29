#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "ogl/oglShader.h"
#include "d3d/d3dShader.h"

namespace byhj
{
	class Triangle 
	{
	public:

	Triangle()  = default;
	~Triangle() = default;

	void init();
	void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
	void update();
	void render();
	void shutdown();
//	void render(ID3D11DeviceContext *pD3D11DeviceContext, const d3d::MatrixBuffer &matrix);

	private:

		void init_buffer();
		void init_vertexArray();
		void init_shader();
		void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
		void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);

	    GLuint m_program;
	    GLuint m_vao;
	    GLuint m_ibo;
	    GLuint m_vbo;
	    std::vector<GLfloat> m_VertexData;
	    std::vector<GLuint>  m_IndexData;
	    OGLShader m_TriangleShader ={ "TriangleShader" };

		ComPtr<ID3D11InputLayout> m_pInputLayout;
		ComPtr<ID3D11Buffer> m_pMVPBuffer;
		ComPtr<ID3D11Buffer> m_pVertexBuffer;
		ComPtr<ID3D11Buffer> m_pIndexBuffer;
	};
}
#endif