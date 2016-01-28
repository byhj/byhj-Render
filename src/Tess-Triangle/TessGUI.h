#ifndef TESSGUI_H
#define TESSGUI_H

#include <glm/glm.hpp>
#include <gl/glew.h>

#include "Gui.h"


namespace byhj
{

	class TessGui : public Gui
	{
	public:
		TessGui()  = default;
		~TessGui() = default;

		void v_init(int sw, int sh)  override;
		void v_render() override;
		void v_update() override;
		void v_shutdown() override;

		bool   getWireFrame() const;
		GLuint getTessInnder() const;
		GLuint getTessOuter() const;

		glm::vec3 getLightDir() const;
		glm::vec4 getAmbientMat() const;
		glm::vec4 getDiffuseMat() const;

	private:
		GLboolean m_WireFrame = false;
		GLuint m_TessInner = 3;
		GLuint m_TessOuter = 2;
		glm::vec4 m_AmbientMat = glm::vec4(0.04f, 0.04f, 0.04f, 1.0f);
		glm::vec4 m_DiffuseMat = glm::vec4(0.0f, 0.75, 0.75, 1.0f);
		glm::vec3 m_LightDir =   glm::vec3(0.25f, 0.25f, -1.0f);
	};
}
#endif
