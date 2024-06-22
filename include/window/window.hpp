#pragma once

#include <util/common.hpp>

class Camera;

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

        const float _cameraSpeed = 0.05f;
        std::unique_ptr<Camera> _camera;
};