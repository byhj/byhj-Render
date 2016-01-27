#ifndef CubeGui_H
#define CubeGui_H

#include "gui.h"

namespace byhj
{
	class CubeGui : public Gui
	{
	public:
		CubeGui() = default;
		~CubeGui() = default;

		void v_init(int sw, int sh) override;

	private:
		TwBar *m_pBar = nullptr;
	};
}
#endif