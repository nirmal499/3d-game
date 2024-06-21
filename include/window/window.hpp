#pragma once

#include <util/common.hpp>

class Window
{
    public:
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
        GLFWwindow* m_window = nullptr;
};