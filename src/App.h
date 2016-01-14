#ifndef App_H
#define App_H

namespace byhj
{
	class App
	{
	public:
		App();
		App(const App &app);
		virtual ~App();

		virtual void v_init()      = 0;
		virtual void v_update()    = 0;
		virtual void v_render()    = 0;
		virtual void v_shutdown()  = 0;

	private:

	};
}
#endif