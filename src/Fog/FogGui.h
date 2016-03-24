#ifndef FogGui_H
#define FogGui_H

#include <gl/glew.h>

#include "gui.h"

#include <glm/glm.hpp>
#include <AntTweakBar.h>
#pragma comment(lib, "AntTweakBar")

namespace byhj {
	typedef enum { FOG_LINEAR = 0, FOG_EXP, FOG_EXP2 } FogModel;
	typedef enum { BASE_PLANE = 0, BASE_RANGE} BaseModel;

	class FogGui : public Gui {
	public:
		FogGui()  = default;
		~FogGui() = default;

			void v_init() override;
			void v_render() override;
			void v_update() override;
			void v_shutdown() override;

			float getFogDensity();
			float getFogStart();
			float getFogEnd();
			int   getBaseModel();
			int   getFogModel();

			float* getFogColor(); 


	private:
		   
		FogModel m_fogModel = FOG_LINEAR;
		BaseModel m_baseModel =BASE_PLANE;
		float m_fogDensity = 1.0f;
		float m_fogStart = -10.0f;
		float m_fogEnd = 10.0f; 
		glm::vec4 m_fogColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	};
}

#endif

