#ifndef app_H
#define app_H


namespace byhj
{
	class App
	{
	public:
		App() = default;
		virtual ~App() = default;

		virtual void v_init() = 0;
		virtual void v_update() = 0;
		virtual void v_render() = 0;
		virtual void v_shutdown() = 0;

		virtual void v_run() = 0;

	};
}
#endif