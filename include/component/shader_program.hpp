#pragma once

#include <util/common.hpp>
#include <string>

class ShaderProgram /* This is an abstract class */
{
    public:
        ShaderProgram(const char* vertexFile, const char* fragmentFile);
        int LoadShader(const char* filename, GLuint shaderType);
        std::string ReadShader(const char *filename);
        GLuint CreateShader(GLenum shaderType, const std::string& source, const char* shaderName);
        void CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename);
        void BindAttribute(GLuint attribute, const char* variableName);
        GLuint GetUniformLocation(const char* variableName);

        void LoadDataFloat(GLuint location, float value);
        void LoadDataBoolean(GLuint location, bool value);
        void LoadDataMatrix(GLuint location, const glm::mat4& matrix);
        void LoadDataVector(GLuint location, const glm::vec3& vector);
        
        void CleanUp();
        void Start();
        void Stop();

        virtual void BindAttributes() = 0;
        virtual void PrefetchAllUniformLocations() = 0;
        void Initialize(); /* It will be used for Second phase initialization */


    private:
        GLuint _shaderProgramID;
        std::string _vertexFilePath;
        std::string _fragmentFilePath;
};