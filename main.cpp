#include <memory>

#include <window/window.hpp>
#include <util/logger.hpp>

int main(int argc, char *argv[])
{
	Window w;
	
	if(!w.Initialization(1280, 720, "Renderer"))
	{
		Logger::log(1, "%s error: Window init error\n", __FUNCTION__);
		return -1;
	}

	w.MainLoop();

	w.CleanUp();

	return 0;
}