#ifndef LightGUI_H
#define LightGUI_H

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "gui.h"
#include "AntTweakBar.h"
#pragma comment(lib, "AntTweakBar")

namespace byhj
{	
	// This example displays one of the following shapes
	typedef enum { LIGHT_PHONG = 0, LIGHT_BLINNE } LightModel;

	class LightGui: public Gui
	{
	public:
		LightGui() = default;
		~LightGui() = default;

		void v_init() override;
		void v_render() override;
		void v_update() override;
		void v_shutdown() override;
        glm::vec3 getLightDir();

	private:
	

		glm::vec3 m_lightDir = glm::vec3(0.5, -3.0, -1.0);
		GLboolean  m_Gamma = false;
		LightModel m_LightModel = LIGHT_PHONG;
	};
}
#endif