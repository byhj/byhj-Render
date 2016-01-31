#ifndef Render_H
#define Render_H

#include <windows.h>
#include <glm/glm.hpp>

namespace byhj
{
	class Render
	{
	public:
		Render() = default;
		virtual ~Render() = default;

		virtual void v_init() {}
		virtual void v_init(HWND hWnd) {}
		virtual void v_update() {};
		virtual void v_update(const glm::mat4 &camMat) {};
		virtual void v_render()    = 0;
		virtual void v_shutdown()  = 0;


	};
}
#endif