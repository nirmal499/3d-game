#pragma once

#include <glad/glad.h>
#include <string>

class ShaderProgram /* This is an abstract class */
{
    public:
        ShaderProgram(const char* vertexFile, const char* fragmentFile);
        int LoadShader(const char* filename, GLuint shaderType);
        std::string ReadShader(const char *filename);
        GLuint CreateShader(GLenum shaderType, const std::string& source, const char* shaderName);
        GLuint CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename);
        void BindAttribute(int arrtibute, const char* variableName);
        void CleanUp();
        void Start();
        void Stop();

        virtual void BindAttributes() = 0;
        void Initialize(); /* It will be used for Second phase initialization */


    private:
        GLuint _shaderProgramID;
        std::string _vertexFilePath;
        std::string _fragmentFilePath;
};