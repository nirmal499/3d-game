#pragma once

#include <component/shader_program.hpp>
#include <util/common.hpp>

class Light;

class StaticShader : public ShaderProgram
{
    public:
        StaticShader();
        void BindAttributes() override;
        void PrefetchAllUniformLocations() override;
        void LoadModelMatrix(const glm::mat4& matrix);
        void LoadViewMatrix(const glm::mat4& matrix);
        void LoadProjectionMatrix(const glm::mat4& matrix);
        void LoadLight(Light* light);

    private:
        GLuint _modelMatrixLocation;
        GLuint _viewMatrixLocation;
        GLuint _projectionMatrixLocation;
        GLuint _lightPositionLocation;
        GLuint _lightColourLocation;


};