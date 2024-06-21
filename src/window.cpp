#include <window/window.hpp>
#include <util/logger.hpp>

bool Window::Initialization(unsigned int width, unsigned int height, std::string title)
{

	if (!glfwInit())
  	{
    	Logger::log(1, "%s: glfwInit() error\n", __FUNCTION__);
    	return false;
  	}

  	/* set a "hint" for the NEXT window created*/
  	// glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

  	if (!m_window)
  	{
    	Logger::log(1, "%s: Could not create window\n", __FUNCTION__);
    	glfwTerminate();
    	return false;
  	}

  	Logger::log(1, "%s: Window successfully initialized\n", __FUNCTION__);

  	glfwMakeContextCurrent(m_window);

  	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  	{
      	Logger::log(1, "%s: Failed to initialize GLAD\n", __FUNCTION__);
      	return false;
  	}

  	Logger::log(1, "%s: Successfully initialized GLAD\n", __FUNCTION__);


	/* save the pointer to the instance as user pointer. m_window is a mandatory parameter in every glfwWindow functions */
	glfwSetWindowUserPointer(m_window, this);

	glfwSetWindowPosCallback(m_window, [](GLFWwindow* win, int xpos, int ypos){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleWindowMoveEvent(xpos, ypos);
	});

	glfwSetWindowIconifyCallback(m_window, [](GLFWwindow* win, int minimized){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleWindowMinimisedEvent(minimized);
	});

	glfwSetWindowMaximizeCallback(m_window, [](GLFWwindow* win, int maximized){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleWindowMaximizedEvent(maximized);
	});

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* win){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleWindowCloseEvent();
	});

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* win, int width, int height){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleWindowResizeEvent(width, height);
		glViewport(0, 0, width, height);
	});

	glfwSetKeyCallback(m_window, [](GLFWwindow *win, int key, int scancode, int action, int mods) {
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleKeyEvents(key, scancode, action, mods);
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* win, int button, int action, int mods){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleMouseButtonEvent(button, action, mods);
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* win, double xpos, double ypos){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleMousePositionEvent(xpos, ypos);
	});

	glfwSetCursorEnterCallback(m_window, [](GLFWwindow* win, int enter){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleMouseEnterLeaveEvent(enter);
	});

  	return true;
}

/*
    double tap in trackpad -> right mouse pressed, right mouse released
    single tap in trackpad -> left mouse pressed, left mouse release
*/
void Window::HandleMouseButtonEvent(int button, int action, int mods)
{
	std::string actionName;
	switch (action)
	{
		case GLFW_PRESS:
			actionName = "pressed";
			break;
		case GLFW_RELEASE:
			actionName = "released";
			break;
		case GLFW_REPEAT:
			actionName = "repeated";
			break;
		default:
			actionName = "invalid";
			break;
	}

  	std::string mouseButtonName;
	switch(button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:
			mouseButtonName = "left";
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			mouseButtonName = "middle";
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			mouseButtonName = "right";
			break;
		default:
			mouseButtonName = "other";
			break;
	}

  	Logger::log(1, "%s: %s mouse button (%i) %s\n", __FUNCTION__, mouseButtonName.c_str(), button, actionName.c_str());
}

/*
  It gives the mouse position when the mouse is in the GLFWwindow screen
*/
void Window::HandleMousePositionEvent(double xpos, double ypos)
{
	Logger::log(1, "%s: Mouse is at position %lf/%lf\n", __FUNCTION__, xpos, ypos);
}

/*
  It is triggered when mouse leaves or enters the GLFWwindow screen
*/
void Window::HandleMouseEnterLeaveEvent(int enter)
{
	if (enter)
	{
		Logger::log(1, "%s: Mouse entered window\n", __FUNCTION__);
	}
	else
	{
		Logger::log(1, "%s: Mouse left window\n", __FUNCTION__);
	}
}

/*
  It is triggered when GLFWwindow screen is resized
*/
void Window::HandleWindowResizeEvent(int width, int height)
{
  	Logger::log(1, "%s: Window has been resized to %i/%i\n", __FUNCTION__, width, height);
}

/*
  It is triggered when a key is pressed and release or keep pressed{ repeated }
*/
void Window::HandleKeyEvents(int key, int scancode, int action, int mods)
{
	std::string actionName;

	switch (action)
	{
		case GLFW_PRESS:
			actionName = "pressed";
			break;
		case GLFW_RELEASE:
			actionName = "released";
			break;
		case GLFW_REPEAT:
			actionName = "repeated";
			break;
		default:
			actionName = "invalid";
			break;
	}

  	const char *keyName = glfwGetKeyName(key, 0);
  	Logger::log(1, "%s: key %s (key %i, scancode %i) %s\n", __FUNCTION__, keyName, key, scancode, actionName.c_str());
}

/*
  It is triggered when GLFWwindow screen window is moved around
*/
void Window::HandleWindowMoveEvent(int xpos, int ypos)
{
	Logger::log(1, "%s: Window has been moved to %i/%i\n", __FUNCTION__, xpos, ypos);
}

/*
  It is triggered when GLFWwindow screen window is minimized or restore from minimized
*/
void Window::HandleWindowMinimisedEvent(int minimized)
{
	if (minimized)
	{
		Logger::log(1, "%s: Window has been minimized\n", __FUNCTION__);
	}
	else
	{
		Logger::log(1, "%s: Window has been restored\n", __FUNCTION__);
	}
}

/*
  It is triggered when GLFWwindow screen window is maximized or restore from maximized
*/
void Window::HandleWindowMaximizedEvent(int maximized)
{
	if (maximized)
	{
		Logger::log(1, "%s: Window has been maximized\n", __FUNCTION__);
	}
	else
	{
		Logger::log(1, "%s: Window has been restored\n", __FUNCTION__);
	}
}
/*
  It is triggered when GLFWwindow screen window is closed
*/
void Window::HandleWindowCloseEvent()
{
	Logger::log(1, "%s: Window got close event... bye!\n", __FUNCTION__);
}

void Window::MainLoop()
{
	/*
		Before going into the loop, we will activate the wait for the vertical sync with a call to the GLFW 
		function, glfwSwapInterval(). Without waiting for it, the window might flicker, or tearing 
		might occur, as the update and buffer switch will be done as fast as possible
	*/
	glfwSwapInterval(1);
	float color = 0.0f;
	while (!glfwWindowShouldClose(m_window))
	{

		color >= 1.0f ? color = 0.0f : color += 0.01f;

		glClearColor(color, color, color, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(m_window);
		/* poll events in a loop */
		glfwPollEvents();

	}
}

void Window::CleanUp()
{
	Logger::log(1, "%s: Terminating Window\n", __FUNCTION__);
	glfwDestroyWindow(m_window);
	glfwTerminate();
}