#include <component/static_shader.hpp>

StaticShader::StaticShader()
    : ShaderProgram(SHADER_PATH "shader.vs", SHADER_PATH "shader.fs")
{
    ShaderProgram::Initialize();
}

void StaticShader::BindAttributes()
{
    this->BindAttribute(0, "Position");
}


