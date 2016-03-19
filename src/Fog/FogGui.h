#ifndef FogGui_H
#define FogGui_H

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "gui.h"
#include <AntTweakBar.h>

#pragma comment(lib, "AntTweakBar")

namespace byhj {
	typedef enum { FOG_LINEAR = 0, FOG_EXP, FOG_EXP2 } FogModel;

	class FogGui : public Gui {
	public:
		FogGui()  = default;
		~FogGui() = default;

			void v_init() override;
			void v_render() override;
			void v_update() override;
			void v_shutdown() override;

	private:

			FogModel m_fogModel = FOG_LINEAR;
	};
}

#endif

