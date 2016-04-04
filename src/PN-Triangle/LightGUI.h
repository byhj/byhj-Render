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

		bool getGamma();
		LightModel getLightModel();
		glm::vec3 getLight(int index);

	private:
		
		glm::vec3 m_LightColors[4] =
		{
			glm::vec3(0.5f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.5f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.5f),
			glm::vec3(0.5f, 0.5f, 0.0f)
		};

		GLboolean  m_Gamma = false;
		LightModel m_LightModel = LIGHT_PHONG;
	};
}
#endif