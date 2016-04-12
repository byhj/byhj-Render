#include "vulkanRender.h"
#include "root.h"


int main()
{
	auto vkApp = std::make_shared<byhj::VulkanRender>();
	byhj::Root::getInstance()->setRender(vkApp);
	byhj::Root::getInstance()->BeginScene();
	byhj::Root::getInstance()->EndScene();

	return 0;
}