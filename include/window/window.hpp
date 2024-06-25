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
        void HandleMousePositionEvent(double xpos, double ypos);
        void HandleMouseEnterLeaveEvent(int enter);
        void HandleKeyEvents(int key, int scancode, int action, int mods);
    
    private:
        GLFWwindow* _window = nullptr;
        unsigned int _width;
        unsigned int _height;

        bool _firstMouse;
        int _lastX;
        int _lastY;

        const float _cameraSpeed = 0.5f;
        std::unique_ptr<Camera> _camera;
        std::unique_ptr<Player> _player;
        std::unique_ptr<Loader> _loader;
        std::unique_ptr<OBJLoader> _objLoader;

        glm::vec3 _tempVec3;
        float _tempDistance;
        float _tempDx;
        float _tempDy;
        float _tempDz;

        std::random_device _rd;
        std::default_random_engine _eng;
        std::uniform_real_distribution<float> _distr;

        float _frameTime = 0.0f;
};