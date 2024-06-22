#include <component/static_shader.hpp>
#include <iostream>

StaticShader::StaticShader()
    : ShaderProgram(SHADER_PATH "shader.vs", SHADER_PATH "shader.fs")
{
    ShaderProgram::Initialize();
}

void StaticShader::BindAttributes()
{
    ShaderProgram::BindAttribute(0, "Position");
    ShaderProgram::BindAttribute(1, "TexCoords");
}

void StaticShader::PrefetchAllUniformLocations()
{
    _modelMatrixLocation = ShaderProgram::GetUniformLocation("modelMatrix");
    _viewMatrixLocation = ShaderProgram::GetUniformLocation("viewMatrix");
    _projectionMatrixLocation = ShaderProgram::GetUniformLocation("projectionMatrix");
}

void StaticShader::LoadModelMatrix(const glm::mat4& matrix)
{
    ShaderProgram::LoadDataMatrix(_modelMatrixLocation, matrix);
}

void StaticShader::LoadViewMatrix(const glm::mat4& matrix)
{
    ShaderProgram::LoadDataMatrix(_viewMatrixLocation, matrix);
}

void StaticShader::LoadProjectionMatrix(const glm::mat4& matrix)
{
    ShaderProgram::LoadDataMatrix(_projectionMatrixLocation, matrix);
}


