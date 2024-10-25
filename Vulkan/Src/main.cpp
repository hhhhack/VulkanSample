#include "VulkanBase.h"

int main()
{
	HelloTriangleApplication app;
	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::println("run error {}",e.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}