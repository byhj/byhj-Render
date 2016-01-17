#ifndef TriangleGui_H
#define TriangleGui_H

#include "gui.h"

namespace byhj
{
	class TriangleGui : public Gui
	{
	public:
		TriangleGui() = default;
		~TriangleGui() = default;

		void v_init(int sw, int sh) override;

	private:
		TwBar *m_pBar = nullptr;
	};
}
#endif