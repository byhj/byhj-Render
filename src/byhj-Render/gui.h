#ifndef __GUI_H_
#define __GUI_H_

namespace byhj
{
	class Gui
	{
	public:
		Gui() = default;
		virtual ~Gui() = default;

		virtual void v_init()     = 0;
		virtual void v_update()   = 0;
		virtual void v_render()   = 0;
		virtual void v_shutdown() = 0;

	private:

	};
}
#endif
