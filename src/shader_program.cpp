#include <component/shader_program.hpp>
#include <fstream>
#include <iostream>
#include <vector>

/* 
	To avoid calling a pure virtual function from a constructor or destructor, you should follow these guidelines:

	1. 	Do Not Call Virtual Functions in Constructors or Destructors: Avoid calling any virtual methods from within constructors or destructors.
		The reason is that during the construction of the base class, the derived class part of the object is not yet constructed, so the virtual 
		table (vtable) does not point to the derived class's methods.

	2. 	Use a Two-Phase Initialization: If you need to perform actions that depend on virtual functions, consider using a two-phase 
		initialization process where the second phase can safely call virtual methods after the object is fully constructed.	
*/
ShaderProgram::ShaderProgram(const char* vertexFile, const char* fragmentFile)
{
	_vertexFilePath = std::string(vertexFile);
	_fragmentFilePath = std::string(fragmentFile);
}

void ShaderProgram::Initialize() /* Second phase initialization */
{
	this->CreateProgram(_vertexFilePath.c_str(), _fragmentFilePath.c_str());
}

/* The contents of the shader file are then stored in a string and returned. */
std::string ShaderProgram::ReadShader(const char *filename)
{
	std::string shaderCode;
	std::ifstream file(filename, std::ios::in);

	if (!file.good())
    {
		std::cout << "Can't read file " << filename << std::endl;
		std::terminate();
	}

	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

GLuint ShaderProgram::CreateShader(GLenum shaderType, const std::string& source, const char* shaderName)
{

	int compile_result = 0;

	GLuint shader = glCreateShader(shaderType);
	const char *shader_code_ptr = source.c_str();
	const int shader_code_size = source.size();

	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	//check for errors
	if (compile_result == GL_FALSE)
	{

		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		
		std::vector<char> shader_log(info_log_length);
		
		glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
		std::cout << "ERROR compiling shader: " << shaderName << std::endl << &shader_log[0] << std::endl;
		return 0;
	}

	return shader;
}

void ShaderProgram::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{

	//read the shader files and save the code
	std::string vertex_shader_code = this->ReadShader(vertexShaderFilename);
	std::string fragment_shader_code = this->ReadShader(fragmentShaderFilename);

	GLuint vertex_shader = this->CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
	GLuint fragment_shader = this->CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");

	int link_result = 0;

	//create the program handle, attatch the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	_shaderProgramID = program;
	BindAttributes(); /* Do not use this-> here */

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);

	//check for link errors
	if (link_result == GL_FALSE)
    {
		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		
		std::vector<char> program_log(info_log_length);

		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	PrefetchAllUniformLocations();
}

void ShaderProgram::BindAttribute(GLuint attribute, const char* variableName)
{
	glBindAttribLocation(_shaderProgramID, attribute, variableName);
}

GLuint ShaderProgram::GetUniformLocation(const char* variableName)
{
    return glGetUniformLocation(_shaderProgramID, variableName);
}

void ShaderProgram::LoadDataFloat(GLuint location, float value)
{
    glUniform1f(location, value);
}

void ShaderProgram::LoadDataBoolean(GLuint location, bool value)
{
	float toLoad = 0;
    if(value)
	{
		toLoad = 1;
	}
    glUniform1f(location, toLoad);
}

void ShaderProgram::LoadDataMatrix(GLuint location, const glm::mat4& matrix)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::LoadDataVector(GLuint location, const glm::vec3& vector)
{
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::CleanUp()
{
    this->Stop();
	glDeleteProgram(_shaderProgramID);
}

void ShaderProgram::Start()
{
    glUseProgram(_shaderProgramID);
}

void ShaderProgram::Stop()
{
    glUseProgram(0);
}
