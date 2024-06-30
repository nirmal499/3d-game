#include <cstdint>
#include <util/mmo_common.hpp>
#include <mmo/message.hpp>
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

Window::Window(): _eng(_rd()), _distr(0.0f, 1000.0f)
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
	_lastX = _width/2.0;
	_lastY = _height/2.0;
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
	glfwSetWindowPos(_window, _distr(_eng), _distr(_eng));

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

	_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	std::unique_ptr<RawModel> rawModel = _objLoader->LoadObjModel(RES_PATH "sheep.obj", _loader.get());
	std::unique_ptr<ModelTexture> texture = std::make_unique<ModelTexture>(_loader->LoadTexture(TEXTURE_PATH "sheep.png"), 10, 1);
	std::unique_ptr<TexturedModel> texturedModel = std::make_unique<TexturedModel>(std::move(rawModel), std::move(texture), "sheep");
	std::unique_ptr<Player> player = std::make_unique<Player>(std::move(texturedModel), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 0, 0), 1.0);

	_playerRecord.insert({1, std::move(player)}); /* The user will always be player No1 */
	_client = std::make_unique<Client>();
	if(_client->Connect("127.0.0.1", 60000))
	{
		std::cout << "Client Successfully sent connection request.\n";
	}
	else
	{
		std::cout << "Client failed sending connection\n";
		std::abort();
	}

	_firstMouse = true;
	_isCursorDisabled = true;
	
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

  	// Logger::log(1, "%s: %s mouse button (%i) %s\n", __FUNCTION__, mouseButtonName.c_str(), button, actionName.c_str());
}

/*
  It gives the mouse position when the mouse is in the GLFWwindow screen
*/
void Window::HandleMousePositionEvent(double xPos, double yPos)
{
	if(!_isCursorDisabled)
	{
		return;
	}

	_xPos = static_cast<float>(xPos);
    _yPos = static_cast<float>(yPos);

	if(_firstMouse)
	{
		_lastX = _xPos;
		_lastY = _yPos;
		_firstMouse = false;
	}

	_xoffset = _xPos - _lastX;
	_yoffset = _lastY - _yPos; /* reversed: y ranges bottom to top */
	
	/* save old values*/
  	_lastX = _xPos;
	_lastY = _yPos;

	_xoffset *= _sensitivity;
	_yoffset *= _sensitivity;

	_camera->_yaw += _xoffset;
	_camera->_pitch += _yoffset;

    if (_camera->_pitch > 89.0f)
	{
		_camera->_pitch = 89.0f;
    }
    if (_camera->_pitch < -89.0f)
	{
		_camera->_pitch = -89.0f;
    }

	/* save old values*/
  	_lastX = xPos;
	_lastY = yPos;

	_tempFloat1 = glm::radians(_camera->_yaw);
	_tempFloat2 = glm::radians(_camera->_pitch);

	_tempFloat3 = glm::cos(_tempFloat1);		/* cos{yaw} */
	_tempFloat4 = glm::sin(_tempFloat1);		/* sin{yaw} */
	_tempFloat5 = glm::cos(_tempFloat2);		/* cos{pitch} */
	_tempFloat6 = glm::sin(_tempFloat2);		/* sin{pitch} */

	/* update view direction */
	_camera->_viewDirection = glm::normalize(
		glm::vec3(
			_tempFloat5 * _tempFloat3,
			_tempFloat6,
			_tempFloat5 * _tempFloat4
		)
	);

	// Logger::log(1, "%s: Mouse is at position %lf/%lf\n", __FUNCTION__, xPos, yPos);
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
	_cameraSpeed = static_cast<float>(2.5 * _deltaTime);

	switch (action)
	{
		case GLFW_PRESS:
		case GLFW_REPEAT:
			// actionName = "pressed";
			{
				switch (key)
				{
				case GLFW_KEY_W:
                	// std::cout << "Key W is pressed" << std::endl;
					_camera->_cameraWorldPosition += _cameraSpeed * _camera->_viewDirection;
                	break;
            	case GLFW_KEY_S:
                	// std::cout << "Key S is pressed" << std::endl;
					_camera->_cameraWorldPosition -= _cameraSpeed * _camera->_viewDirection;
					break;
            	case GLFW_KEY_D:
                	// std::cout << "Key D is pressed" << std::endl;
					_camera->_cameraWorldPosition += _cameraSpeed * glm::normalize(glm::cross(_camera->_viewDirection, _camera->_worldUpVector));
                	break;
            	case GLFW_KEY_A:
                	// std::cout << "Key A is pressed" << std::endl;
					_camera->_cameraWorldPosition -= _cameraSpeed * glm::normalize(glm::cross(_camera->_viewDirection, _camera->_worldUpVector));
                	break;
				case GLFW_KEY_E:
                	// std::cout << "Key E is pressed" << std::endl;
					_camera->_cameraWorldPosition += _cameraSpeed * _camera->_worldUpVector;
                	break;
				case GLFW_KEY_Q:
                	// std::cout << "Key Q is pressed" << std::endl;
					_camera->_cameraWorldPosition -= _cameraSpeed * _camera->_worldUpVector;
                	break;
				case GLFW_KEY_O:
                	// std::cout << "Key O is pressed" << std::endl;
					_playerRecord[1]->_currentSpeed = Player::RUN_SPEED;
					isOKKey = true;
                	break;
            	case GLFW_KEY_K:
                	// std::cout << "Key K is pressed" << std::endl;
					_playerRecord[1]->_currentSpeed = -Player::RUN_SPEED;
					isOKKey = true;
					break;
            	case GLFW_KEY_J:
                	// std::cout << "Key J is pressed" << std::endl;
					_playerRecord[1]->_currentTurnSpeed = Player::TURN_SPEED;
					isJLKey = true;
                	break;
            	case GLFW_KEY_L:
                	// std::cout << "Key L is pressed" << std::endl;
					_playerRecord[1]->_currentTurnSpeed = -Player::TURN_SPEED;
					isJLKey = true;
                	break;
				// case GLFW_KEY_G:
                // 	// std::cout << "Key G is pressed" << std::endl;
				// 	if(!_connectionExists && !_client->IsConnected())
				// 	{
				// 		/*
				// 			Eventhough socket is closed meaning "!_client->IsConnected()" gives true;
				// 			The thread in the client may still be running we need to first clean the client
				// 			by calling client->Disconnect()
				// 		*/
				// 		if(_client->Connect("127.0.0.1", 60000))
				// 		{
				// 			std::cout << "Client Successfully sent connection request.\n";
				// 			_connectionExists = true;
				// 		}
				// 		else
				// 		{
				// 			std::cout << "Client failed sending connection\n";
				// 		}
				// 	}
				// 	else
				// 	{
				// 		std::cout << "Client socket maybe closed, but the thread running the client is still there, So trying reconnecting by pressing H\n";
				// 	}
                // 	break;
				// case GLFW_KEY_H:
				// 	if(_connectionExists && !_client->IsConnected())
				// 	{
				// 		/* socket is closed */

				// 		std::cout << "Trying to reconnect client to server\n";
				// 		_client->Reconnect();
				// 	}
				// 	break;
				case GLFW_KEY_P:
                	// std::cout << "Key G is pressed" << std::endl;
					if(_client->IsConnected())
					{
						_client->PingServer();
					}
					else
					{
						std::cout << "Client is not connected\n";
					}
                	break;
				case GLFW_KEY_V:
					if(!_startsMMO)
					{
						std::cout << "Starting MMO.......\n";
						_startsMMO = true;
					}
					else
					{
						std::cout << "MMO is already started....\n";
					}
					break;
				case GLFW_KEY_R:
					_playerRecord[1]->SetPosition({0.0f, 0.0f, 0.0f});
					break;
				case GLFW_KEY_N:
					_camera->_cameraWorldPosition = glm::vec3(0.0f, 0.0f, 3.0f);
					_camera->_viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
					_camera->_worldUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
					_camera->_yaw = -90.0f;
					_camera->_pitch = 0.0f;
					break;
				case GLFW_KEY_T:
					{
						_isCursorDisabled = !_isCursorDisabled;
						if(_isCursorDisabled)
						{
							glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						}
						else
						{
							glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
							_firstMouse = true;
						}
					}
					break;
				case GLFW_KEY_ESCAPE:
					glfwSetWindowShouldClose(_window, true);
					break;
            	default:
					_playerRecord[1]->_currentSpeed = 0;
					_playerRecord[1]->_currentTurnSpeed = 0;
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

	if(isJLKey)
	{
		_tempVec3.x = 0.0f;
		_tempVec3.y = (_playerRecord[1]->_currentTurnSpeed * Player::TURN_SPEED) * _cameraSpeed;
		_tempVec3.z = 0.0f;
		_playerRecord[1]->IncreaseRotation(_tempVec3);
	}
	
	if(isOKKey)
	{
		_tempFloat1 = (_playerRecord[1]->_currentSpeed * Player::RUN_SPEED) * _cameraSpeed;
		_tempFloat2 = static_cast<float>(_tempFloat1 * glm::sin(glm::radians(_playerRecord[1]->GetRotation().y)));
		_tempFloat3 = static_cast<float>(_tempFloat1 * glm::cos(glm::radians(_playerRecord[1]->GetRotation().y)));
		_tempVec3.x = _tempFloat2;
		_tempVec3.y = 0.0f;
		_tempVec3.z = _tempFloat3;
		_playerRecord[1]->IncreasePosition(_tempVec3);

		// std::cout << "Current Player Position: " << _playerRecord[1]->GetPosition().x << " " << _playerRecord[1]->GetPosition().y<< " " << _playerRecord[1]->GetPosition().z << "\n";
	}

  	// const char *keyName = glfwGetKeyName(key, 0);
  	// Logger::log(1, "%s: key %s (key %i, scancode %i) %s\n", __FUNCTION__, keyName, key, scancode, actionName.c_str());
}

/*
  It is triggered when GLFWwindow screen window is moved around
*/
void Window::HandleWindowMoveEvent(int xpos, int ypos)
{
	// Logger::log(1, "%s: Window has been moved to %i/%i\n", __FUNCTION__, xpos, ypos);
}

/*
  It is triggered when GLFWwindow screen window is minimized or restore from minimized
*/
void Window::HandleWindowMinimisedEvent(int minimized)
{
	if (minimized)
	{
		// Logger::log(1, "%s: Window has been minimized\n", __FUNCTION__);
	}
	else
	{
		// Logger::log(1, "%s: Window has been restored\n", __FUNCTION__);
	}
}

/*
  It is triggered when GLFWwindow screen window is maximized or restore from maximized
*/
void Window::HandleWindowMaximizedEvent(int maximized)
{
	if (maximized)
	{
		// Logger::log(1, "%s: Window has been maximized\n", __FUNCTION__);
	}
	else
	{
		// Logger::log(1, "%s: Window has been restored\n", __FUNCTION__);
	}
}
/*
  It is triggered when GLFWwindow screen window is closed
*/
void Window::HandleWindowCloseEvent()
{
	// Logger::log(1, "%s: Window got close event... bye!\n", __FUNCTION__);
}

void Window::SendData()
{
	if(_client->IsConnected())
	{
		const glm::vec3& playerPosition = _playerRecord[1]->GetPosition();
		const glm::vec3& playerRotation = _playerRecord[1]->GetRotation();

		_playerPositionAndRotationInfo[0] = playerPosition.x;
		_playerPositionAndRotationInfo[1] = playerPosition.y;
		_playerPositionAndRotationInfo[2] = playerPosition.z;

		_playerPositionAndRotationInfo[3] = playerRotation.x;
		_playerPositionAndRotationInfo[4] = playerRotation.y;
		_playerPositionAndRotationInfo[5] = playerRotation.z;
		
		_client->MessageAll(_playerPositionAndRotationInfo);
	}
}

void Window::PollReponse()
{
	if(_client->IsConnected())
	{
		while(!_client->IncomingMsgQ().Empty())
		{
			auto msg = _client->IncomingMsgQ().PopFrontWithoutSleep()._msg;

			switch(msg._header._id)
			{
			case CustomMsgTypes::ServerPing:
				{
					std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point timeThen;

					msg >> timeThen;

					/* Server has responded to a ping request */
					std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << " seconds\n";
				}
				break;

			case CustomMsgTypes::ServerAccept:
				{
					std::cout << "Server Accepted Connection\n";
					
					// uint32_t noOfClientsConnectedRightNow;
					// msg >> noOfClientsConnectedRightNow;

					// std::cout << "Right Now " << noOfClientsConnectedRightNow << " are connected\n";

					// uint32_t clientID;
					// int counter = 0;
					// while(counter <= noOfClientsConnectedRightNow)
					// {
					// 	msg >> clientID;
					// 	_tempVectorUINT.push_back(clientID);
					// 	counter++;
					// }

					// for(const auto& playerID: _tempVectorUINT)
					// {
					// 	std::unique_ptr<TexturedModel> model = std::make_unique<TexturedModel>(*(_playerRecord[1]->GetModel()));
					// 	std::unique_ptr<Player> newPlayer = std::make_unique<Player>(std::move(model), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 0, 0), 1.0);
					// 	_playerRecord.insert({playerID, std::move(newPlayer)});
					// }
				}
				break;
			// case CustomMsgTypes::NewConnectionAdded:
			// 	{
			// 		uint32_t playerID;
			// 		msg >> playerID;

			// 		std::cout << "Adding New Player " << playerID << "\n";

			// 		std::unique_ptr<TexturedModel> model = std::make_unique<TexturedModel>(*(_playerRecord[1]->GetModel()));
			// 		std::unique_ptr<Player> newPlayer = std::make_unique<Player>(std::move(model), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 0, 0), 1.0);
			// 		_playerRecord.insert({playerID, std::move(newPlayer)});
			// 	}
			// 	break;
			case CustomMsgTypes::ServerMessage:
				{
					uint32_t playerIDToUpdate;
					msg >> playerIDToUpdate;

					/* Get the position information of all connected player and update it for current client */
					msg >> _playerPositionAndRotationInfo;

					_playerPositionInfo[0] = _playerPositionAndRotationInfo[0];
					_playerPositionInfo[1] = _playerPositionAndRotationInfo[1];
					_playerPositionInfo[2] = _playerPositionAndRotationInfo[2];

					_playerRotationInfo[0] = _playerPositionAndRotationInfo[3];
					_playerRotationInfo[1] = _playerPositionAndRotationInfo[4];
					_playerRotationInfo[2] = _playerPositionAndRotationInfo[5];

					auto it = _playerRecord.find(playerIDToUpdate);
					if(it != _playerRecord.end())
					{
						it->second->SetPosition(_playerPositionInfo);
						it->second->SetRotation(_playerRotationInfo);

						// std::cout << "Successfully updated position and rotation for playerID " << playerIDToUpdate << "\n"; 
					}
					else
					{
						std::unique_ptr<TexturedModel> model = std::make_unique<TexturedModel>(*(_playerRecord[1]->GetModel()));
						std::unique_ptr<Player> newPlayer = std::make_unique<Player>(std::move(model), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 0, 0), 1.0);
						newPlayer->SetPosition(_playerPositionInfo);

						_playerRecord.insert({playerIDToUpdate, std::move(newPlayer)});
						std::cout << playerIDToUpdate << " Player Does not Exists, so create it and We tried to change its position\n";
					}
				}
				break;
			case CustomMsgTypes::ClientsDisconnected:
				{
					uint32_t sizeOfVector; 
					msg >> sizeOfVector;

					std::cout << "No. Of Clients Disconnected : " << sizeOfVector << "\n";
					
					int counter = 1;
					uint32_t clientID;
					while(counter <= sizeOfVector)
					{	
						msg >> clientID;
						std::cout << "Client ID : " << clientID << " disconnected\n";
						_tempVectorUINT.push_back(clientID);
						++counter;
					}

					for(const auto& ID: _tempVectorUINT)
					{
						_playerRecord.erase(ID); /* It does not throws when key is not present. */
						/* key might not be present */
					}
				}
				break;
			default:
				break;
			}

		}
	}
}

void Window::MainLoop()
{
	/*
		Before going into the loop, we will activate the wait for the vertical sync with a call to the GLFW 
		function, glfwSwapInterval(). Without waiting for it, the window might flicker, or tearing 
		might occur, as the update and buffer switch will be done as fast as possible
	*/
	glfwSwapInterval(1);

	std::unique_ptr<Light> light = std::make_unique<Light>(glm::vec3(0, 0, -5), glm::vec3(1.0, 1.0, 1.0));

	std::unique_ptr<MasterRenderer> masterRenderer = std::make_unique<MasterRenderer>(_width, _height);

	while (!glfwWindowShouldClose(_window))
	{
		_currentFrame = static_cast<float>(glfwGetTime());
        _deltaTime = _currentFrame - _lastFrame;
        _lastFrame = _currentFrame;

		if(_startsMMO)
		{
			this->PollReponse(); 	/* Get other players position */
		}

		/*
			std::cout << "Camera View: " << _camera->_yaw << " " << _camera->_pitch << "\n";
			std::cout << "Camera Position: " << _camera->_cameraWorldPosition.x << " " << _camera->_cameraWorldPosition.y << " " << _camera->_cameraWorldPosition.z << "\n";
			std::cout << "Camera View Direction: " << _camera->_viewDirection.x << " " << _camera->_viewDirection.y << " " << _camera->_viewDirection.z << "\n";
			std::cout << "Player Position: " << _playerRecord[1]->GetPosition().x << " " << _playerRecord[1]->GetPosition().y<< " " << _playerRecord[1]->GetPosition().z << "\n";
		*/

		/* Render the changed position of all players in the world accordingly in the BACK BUFFER */
		for(const auto& player:_playerRecord)
		{
			masterRenderer->ProcessEntity(player.second.get());
		}
		masterRenderer->Render(light.get(), _camera.get());

		glfwSwapBuffers(_window); 	/* Swap the back buffer */
		glfwPollEvents();					/* Client make the changes in his/her position after analysing the rendered BACK BUFFER */
		
		if(_startsMMO)
		{
			this->SendData();					/* Client Transmits his/her position details to the server */
		}
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
