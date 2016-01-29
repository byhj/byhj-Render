#ifndef Render_H
#define Render_H

#include <windows.h>

namespace byhj
{
	class Render
	{
	public:
		Render() = default;
		virtual ~Render() = default;

		virtual void v_init() {}
		virtual void v_init(HWND hWnd) {}
		virtual void v_update()    = 0;
		virtual void v_render()    = 0;
		virtual void v_shutdown()  = 0;


	};
}
#endif