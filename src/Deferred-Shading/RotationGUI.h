#ifndef RotationGui_H
#define RotationGui_H

#include "gui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

namespace byhj
{

class RotationGui : public Gui
{
public:
	 RotationGui() = default;
	 ~RotationGui() = default;
    
	 void v_init(int sw, int sh) override; 
	 void v_render() override;
	 void v_shutdown() override;

	 glm::mat4 getRotationMat()
	 {
		 return glm::mat4_cast(m_Orientation);
	 }
private:
	glm::quat m_Orientation;
};

}
#endif