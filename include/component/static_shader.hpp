#pragma once

#include <component/shader_program.hpp>
#include <util/common.hpp>

class StaticShader : public ShaderProgram
{
    public:
        StaticShader();
        void BindAttributes() override;
};