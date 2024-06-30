#pragma once

#include <util/common.hpp>
#include <random>
#include <net/client.hpp>

class Camera;
class Player;
class Loader;
class OBJLoader;

class Window
{
    public:
        Window();
        ~Window();
        bool Initialization(unsigned int width, unsigned int height, std::string title);
        void MainLoop();
        void CleanUp();
    
    private:
        void HandleWindowMoveEvent(int xpos, int ypos);
        void HandleWindowMinimisedEvent(int minimized);
        void HandleWindowMaximizedEvent(int maximized);
        void HandleWindowCloseEvent();
        void HandleWindowResizeEvent(int width, int height);
        void HandleMouseButtonEvent(int button, int action, int mods);
        void HandleMousePositionEvent(double xPos, double yPos);
        void HandleMouseEnterLeaveEvent(int enter);
        void HandleKeyEvents(int key, int scancode, int action, int mods);

        void CalculateCameraPosition();
        void PollReponse();
        void SendData();
    
    private:
        GLFWwindow* _window = nullptr;
        unsigned int _width;
        unsigned int _height;

        std::unique_ptr<Camera> _camera;
        std::unique_ptr<Loader> _loader;
        std::unique_ptr<OBJLoader> _objLoader;

        /* Some temporay variables */
        glm::vec3 _tempVec3;
        float _tempFloat1;
        float _tempFloat2;
        float _tempFloat3;
        float _tempFloat4;
        float _tempFloat5;
        float _tempFloat6;
        /***************************** */

        float _cameraSpeed = 0.0f;
        float _deltaTime = 0.0f; /* time between current frame and last frame */
        float _lastFrame = 0.0f;
        float _currentFrame = 0.0f;

        std::random_device _rd;
        std::default_random_engine _eng;
        std::uniform_real_distribution<float> _distr;

        const float _sensitivity = 0.1f;
        float _lastX;
        float _lastY;
        float _xoffset;
        float _yoffset;
        float _xPos;
        float _yPos;
        bool _firstMouse;

        std::unordered_map<uint32_t, std::unique_ptr<Player>> _playerRecord;
        std::unique_ptr<Client> _client = nullptr;
        bool _connectionExists = false;
        bool _startsMMO = false;
        std::array<float, 6> _playerPositionAndRotationInfo;
        std::array<float, 3> _playerPositionInfo;
        std::array<float, 3> _playerRotationInfo;

        std::vector<uint32_t> _tempVectorUINT;

        bool _isCursorDisabled;
};