#ifndef Render_H
#define Render_H

namespace byhj
{
	class Render
	{
	public:
		Render() = default;
		virtual ~Render() = default;

		virtual void v_init()      = 0;
		virtual void v_update()    = 0;
		virtual void v_render()    = 0;
		virtual void v_shutdown()  = 0;


	};
}
#endif