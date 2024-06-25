#pragma once


// GLAD needs to be included first
#include <glad/glad.h>

// GLFW is included next
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <memory>

#define SHADER_PATH "/home/nbaskey/Desktop/nirmal/projects/cg/ogl-engine-copy/assets/shader/"
#define TEXTURE_PATH "/home/nbaskey/Desktop/nirmal/projects/cg/ogl-engine-copy/assets/texture/"
#define RES_PATH "/home/nbaskey/Desktop/nirmal/projects/cg/ogl-engine-copy/assets/res/"

struct ProjectionDetails
{
    float _fov;
    int _width;
    int _height;
    float _zNear;
    float _zFar;

    ProjectionDetails(float fov, int width, int height, float zNear, float zFar)
    {
        _fov = fov;
        _width = width;
        _height = height;
        _zNear = zNear;
        _zFar = zFar;
    }
};


