#ifndef Render_H
#define Render_H

namespace byhj
{
	class Render
	{
	public:
		Render();
		virtual ~Render();

		virtual void v_init()      {}
		virtual void v_update()    {}
		virtual void v_render()    {}
		virtual void v_shutdown()  {}

  	private:

	};
}
#endif