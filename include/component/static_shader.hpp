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
        void LoadCameraPosition(const glm::vec3& position);
        void LoadShineVariables(float damper, float reflectivity);

    private:
        GLuint _modelMatrixLocation;
        GLuint _viewMatrixLocation;
        GLuint _projectionMatrixLocation;
        GLuint _lightPositionLocation;
        GLuint _lightColourLocation;
        GLuint _cameraPositionLocation;
        GLuint _shineDamperLocation;
        GLuint _reflectivityLocation;



};