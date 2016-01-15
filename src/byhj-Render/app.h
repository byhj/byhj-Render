#ifndef app_H
#define app_H

namespace byhj
{
	class App
	{
	public:
		App();
		App(const App &app);
		virtual ~App();

		virtual void v_init()      {}
		virtual void v_update()    {}
		virtual void v_render()    {}
		virtual void v_shutdown()  {}

	private:

	};
}
#endif