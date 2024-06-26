#pragma once

#include <util/common.hpp>
#include <random>

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
    
    private:
        GLFWwindow* _window = nullptr;
        unsigned int _width;
        unsigned int _height;

        const float _speed = 0.5f;
        std::unique_ptr<Camera> _camera;
        std::unique_ptr<Player> _player;
        std::unique_ptr<Loader> _loader;
        std::unique_ptr<OBJLoader> _objLoader;

        glm::vec3 _tempVec3;
        float _tempFloat1;
        float _tempFloat2;
        float _tempFloat3;
        float _tempFloat4;
        float _tempFloat5;
        float _tempFloat6;

        float _frameStartTime = 0.0f;
        float _prevFrameStartTime = 0.0f;

        std::random_device _rd;
        std::default_random_engine _eng;
        std::uniform_real_distribution<float> _distr;

        float _frameTime = 0.0f;
        float _mouseXPos = 0.0f;
        float _mouseYPos = 0.0f;
        int _moveForward = 0;
        int _moveRight = 0;
        int _moveUp = 0;
};