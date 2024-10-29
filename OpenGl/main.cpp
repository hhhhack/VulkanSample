#include <stdexcept>
#include <print>
#include "OpenGl.h"

int main(int argc, char** argv)
{
	OpenGl opengl;
	try{
		opengl.run();
	}
	catch (const std::exception e){
		std::println("run err is {}", e.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}