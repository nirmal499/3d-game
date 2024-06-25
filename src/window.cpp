#include <component/master_renderer.hpp>
#include <component/entity.hpp>
#include <memory>
#include <vector>
#include <component/loader.hpp>
#include <component/light.hpp>
#include <component/renderer.hpp>
#include <window/window.hpp>
#include <component/raw_model.hpp>
#include <component/static_shader.hpp>
#include <component/model_texture.hpp>
#include <component/textured_model.hpp>
#include <component/obj_loader.hpp>
#include <component/camera.hpp>
#include <component/player.hpp>
#include <util/logger.hpp>
#include <util/math.hpp>
#include <iostream>

Window::Window(): _eng(_rd()), _distr(0.0f, 1.0f)
{
	
}

Window::~Window()
{
	
}

bool Window::Initialization(unsigned int width, unsigned int height, std::string title)
{

	if (!glfwInit())
  	{
    	Logger::log(1, "%s: glfwInit() error\n", __FUNCTION__);
    	return false;
  	}

  	/* set a "hint" for the NEXT window created*/
  	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	_width = width;
	_height = height;
  	_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	
	/*
		we will tell GLFW that it should hide the cursor and capture it. Capturing a cursor means
		that, once the application has focus, the mouse cursor stays within the center of the window (unless
		the application loses focus or quits)
	*/
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  	if (!_window)
  	{
    	Logger::log(1, "%s: Could not create window\n", __FUNCTION__);
    	glfwTerminate();
    	return false;
  	}

  	Logger::log(1, "%s: Window successfully initialized\n", __FUNCTION__);

  	glfwMakeContextCurrent(_window);

  	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  	{
      	Logger::log(1, "%s: Failed to initialize GLAD\n", __FUNCTION__);
      	return false;
  	}

  	Logger::log(1, "%s: Successfully initialized GLAD\n", __FUNCTION__);


	/* save the pointer to the instance as user pointer. _window is a mandatory parameter in every glfwWindow functions */
	glfwSetWindowUserPointer(_window, this);

	/*
	glfwSetWindowPosCallback(_window, [](GLFWwindow* win, int xpos, int ypos){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleWindowMoveEvent(xpos, ypos);
	});

	glfwSetWindowIconifyCallback(_window, [](GLFWwindow* win, int minimized){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleWindowMinimisedEvent(minimized);
	});

	glfwSetWindowMaximizeCallback(_window, [](GLFWwindow* win, int maximized){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleWindowMaximizedEvent(maximized);
	});

	glfwSetWindowCloseCallback(_window, [](GLFWwindow* win){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleWindowCloseEvent();
	});

	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* win, int width, int height){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleWindowResizeEvent(width, height);
		glViewport(0, 0, width, height);
	});
	*/

	glfwSetKeyCallback(_window, [](GLFWwindow *win, int key, int scancode, int action, int mods) {
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleKeyEvents(key, scancode, action, mods);
	});

	/*
	glfwSetMouseButtonCallback(_window, [](GLFWwindow* win, int button, int action, int mods){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleMouseButtonEvent(button, action, mods);
	});

	glfwSetCursorEnterCallback(_window, [](GLFWwindow* win, int enter){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleMouseEnterLeaveEvent(enter);
	});
	*/

	glfwSetCursorPosCallback(_window, [](GLFWwindow* win, double xpos, double ypos){
		auto thisPointerSavedEarlier = static_cast<Window*>(glfwGetWindowUserPointer(win));
		thisPointerSavedEarlier->HandleMousePositionEvent(xpos, ypos);
	});

	_loader = std::make_unique<Loader>();
	_objLoader = std::make_unique<OBJLoader>();

	_camera = std::make_unique<Camera>(glm::vec3(0, 0, 3), glm::vec3(0, 0, -3), glm::vec3(0, 1, 0), -90.0f, 0.0f);

	std::unique_ptr<RawModel> rawModel = _objLoader->LoadObjModel(RES_PATH "sheep.obj", _loader.get());
	std::unique_ptr<ModelTexture> texture = std::make_unique<ModelTexture>(_loader->LoadTexture(TEXTURE_PATH "sheep.png"), 10, 1);
	std::unique_ptr<TexturedModel> texturedModel = std::make_unique<TexturedModel>(std::move(rawModel), std::move(texture), "sheep");
	_player = std::make_unique<Player>(std::move(texturedModel), glm::vec3(0, 0, -20), glm::vec3(0, 0, 0), 1.0);
	
	_firstMouse = true;
	_lastX = _width / 2.0f;
	_lastY = _height / 2.0f;

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
void Window::HandleMousePositionEvent(double xPos, double yPos)
{
	/* calculate relative movement from last position */
	int mouseMoveRelX = static_cast<int>(xPos) - _mouseXPos;
	int mouseMoveRelY = static_cast<int>(yPos) - _mouseYPos;

	_camera->_viewAzimuth += mouseMoveRelX / 10.0;
    /* keep between 0 and 360 degree */
    if (_camera->_viewAzimuth < 0.0)
	{
		_camera->_viewAzimuth += 360.0;
    }
    if (_camera->_viewAzimuth >= 360.0)
	{
		_camera->_viewAzimuth -= 360.0;
    }

    _camera->_viewElevation -= mouseMoveRelY / 10.0;
    /* keep between -89 and +89 degree */
    if (_camera->_viewElevation > 89.0)
	{
		_camera->_viewElevation = 89.0;
    }
    if (_camera->_viewElevation < -89.0)
	{
		_camera->_viewElevation = -89.0;
    }

	/* save old values*/
  	_mouseXPos = static_cast<int>(xPos);
  	_mouseYPos = static_cast<int>(yPos);

	this->CalculateCameraPosition();

	Logger::log(1, "%s: Mouse is at position %lf/%lf\n", __FUNCTION__, xPos, yPos);
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
	// std::string actionName;
	bool isJLKey = false;
	bool isOKKey = false;
	bool isWSDAEQKey = false;

	_moveForward = 0;
    _moveRight = 0;
	_moveUp = 0;

	switch (action)
	{
		case GLFW_PRESS:
		case GLFW_REPEAT:
			// actionName = "pressed";
			{
				switch (key)
				{
				case GLFW_KEY_W:
                	std::cout << "Key W is pressed" << std::endl;
					_moveForward += 1;
					isWSDAEQKey = true;
                	break;
            	case GLFW_KEY_S:
                	std::cout << "Key S is pressed" << std::endl;
					_moveForward -= 1;
					isWSDAEQKey = true;
					break;
            	case GLFW_KEY_D:
                	std::cout << "Key D is pressed" << std::endl;
					_moveRight += 1;
					isWSDAEQKey = true;
                	break;
            	case GLFW_KEY_A:
                	std::cout << "Key A is pressed" << std::endl;
					_moveRight -= 1;
					isWSDAEQKey = true;
                	break;
				case GLFW_KEY_E:
                	std::cout << "Key E is pressed" << std::endl;
					_moveUp += 1;
					isWSDAEQKey = true;
                	break;
				case GLFW_KEY_Q:
                	std::cout << "Key Q is pressed" << std::endl;
					_moveUp -= 1;
					isWSDAEQKey = true;
                	break;
				case GLFW_KEY_O:
                	std::cout << "Key O is pressed" << std::endl;
					_player->_currentSpeed = Player::RUN_SPEED;
					isOKKey = true;
                	break;
            	case GLFW_KEY_K:
                	std::cout << "Key K is pressed" << std::endl;
					_player->_currentSpeed = -Player::RUN_SPEED;
					isOKKey = true;
					break;
            	case GLFW_KEY_J:
                	std::cout << "Key J is pressed" << std::endl;
					_player->_currentTurnSpeed = Player::TURN_SPEED;
					isJLKey = true;
                	break;
            	case GLFW_KEY_L:
                	std::cout << "Key L is pressed" << std::endl;
					_player->_currentTurnSpeed = -Player::TURN_SPEED;
					isJLKey = true;
                	break;
				case GLFW_KEY_ESCAPE:
					glfwSetWindowShouldClose(_window, true);
					break;
            	default:
					_player->_currentSpeed = 0;
					_player->_currentTurnSpeed = 0;
                	break;
				}
			}
			break;
		case GLFW_RELEASE:
			// actionName = "released";
			break;
		// case GLFW_REPEAT:
		// 	actionName = "repeated";
		// 	break;
		default:
			// actionName = "invalid";
			break;
	}

	if(isWSDAEQKey)
	{
		this->CalculateCameraPosition();
	}

	if(isJLKey)
	{
		_tempVec3.x = 0.0f;
		_tempVec3.y = _player->_currentTurnSpeed * _speed;
		_tempVec3.z = 0.0f;
		_player->IncreaseRotation(_tempVec3);
	}
	
	if(isOKKey)
	{
		_tempFloat1 = _player->_currentSpeed * _speed;
		_tempFloat2 = static_cast<float>(_tempFloat1 * glm::sin(glm::radians(_player->GetRotation().y)));
		_tempFloat3 = static_cast<float>(_tempFloat1 * glm::cos(glm::radians(_player->GetRotation().y)));
		_tempVec3.x = _tempFloat2;
		_tempVec3.y = 0.0f;
		_tempVec3.z = _tempFloat3;
		_player->IncreasePosition(_tempVec3);
	}

  	// const char *keyName = glfwGetKeyName(key, 0);
  	// Logger::log(1, "%s: key %s (key %i, scancode %i) %s\n", __FUNCTION__, keyName, key, scancode, actionName.c_str());
}

void Window::CalculateCameraPosition()
{
	_tempFloat1 = glm::radians(_camera->_viewAzimuth);
	_tempFloat2 = glm::radians(_camera->_viewElevation);

	_tempFloat3 = glm::cos(_tempFloat1);		/* cosAzimuth */
	_tempFloat4 = glm::sin(_tempFloat1);		/* sinAzimuth */
	_tempFloat5 = glm::cos(_tempFloat2);		/* cosElevation */
	_tempFloat6 = glm::sin(_tempFloat2);		/* sinElevation */

	/* update view direction */
	_camera->_viewDirection = glm::normalize(
		glm::vec3(
			_tempFloat5 * _tempFloat3,
			_tempFloat6,
			_tempFloat5 * _tempFloat4
		)
	);

	/* calculate right and up direction */
	_camera->_rightDirection = glm::normalize(glm::cross(_camera->_viewDirection, _camera->_worldUpVector));
	_camera->_upDirection = glm::normalize(glm::cross(_camera->_rightDirection, _camera->_viewDirection));

	/* update camera position depending on desired movement */
	_camera->_cameraWorldPosition +=	
			_moveForward * _speed * _camera->_viewDirection
		+ 	_moveRight * _speed * _camera->_rightDirection
		+ 	_moveUp * _speed * _camera->_upDirection;
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

	std::unique_ptr<Light> light = std::make_unique<Light>(glm::vec3(3000, 2000, 3000), glm::vec3(1.0, 1.0, 1.0));

	std::unique_ptr<MasterRenderer> masterRenderer = std::make_unique<MasterRenderer>(_width, _height);

	std::vector<std::unique_ptr<Entity>> allCubes;
	for(int i = 0; i < 200; i++)
	{
		float x = _distr(_eng) * 100 - 50;
		float y = _distr(_eng) * 100 - 50;
		float z = _distr(_eng) * -300;

		float rx = _distr(_eng) * 180.0f;
		float ry = _distr(_eng) * 180.0f;
		float rz = 0.0f;

		std::unique_ptr<TexturedModel> cubeModel = std::make_unique<TexturedModel>(*(_player->GetModel()));
		std::unique_ptr<Entity> cubeEntity = std::make_unique<Entity>(std::move(cubeModel), glm::vec3(x, y, z), glm::vec3(rx, ry, rz), 1.0);
		allCubes.emplace_back(std::move(cubeEntity));
	}

	static float prevFrameStartTime = 0.0f;
	float frameStartTime = 0.0f;
	while (!glfwWindowShouldClose(_window))
	{
  		frameStartTime = glfwGetTime();

		for(const auto& cube: allCubes)
		{
			masterRenderer->ProcessEntity(cube.get());
		}
		masterRenderer->ProcessEntity(_player.get());
		masterRenderer->Render(light.get(), _camera.get());
		
		glfwSwapBuffers(_window);

		_frameTime = frameStartTime - prevFrameStartTime;
  		prevFrameStartTime = frameStartTime;

		/* poll events in a loop */
		glfwPollEvents();

	}
	
	masterRenderer->CleanUp();

	_loader->CleanUp();

}


void Window::CleanUp()
{
	Logger::log(1, "%s: Terminating Window\n", __FUNCTION__);
	glfwDestroyWindow(_window);
	glfwTerminate();
}
