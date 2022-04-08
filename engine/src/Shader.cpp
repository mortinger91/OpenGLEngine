#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

Shader::Shader(const std::string& filepathVertex, const std::string& filepathFragment)
	:m_FilepathVertex(filepathVertex), m_FilepathFragment(filepathFragment), m_RendererID(0)
{
	ShaderProgramSource source;
	//source = ParseShader(m_Filepath);
	source.VertexSource = GetFileContent(m_FilepathVertex);
	source.FragmentSource = GetFileContent(m_FilepathFragment);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

std::string Shader::GetFileContent(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss;
	while (getline(stream, line))
	{
		ss << line << '\n';
	}
	return ss.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	// creates an empty shader object and returns a non-zero value by which it can be referenced
	// A shader object is used to maintain the source code strings that define a shader.
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	//Error handling
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		 int lenght;
		 GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
		 char* message = (char*)alloca(lenght * sizeof(char));
		 GLCall(glGetShaderInfoLog(id, lenght, &lenght, message));
		 std::cout << "Failed to compile shader: " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		 std::cout << message << std::endl;
		 GLCall(glDeleteShader(id));
		 return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// Creates a program object
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	// Specifies the handle of the program object to be validated
	GLCall(glValidateProgram(program));

	// check if the program is correct
	int status;
	GLCall(glGetProgramiv(program, GL_VALIDATE_STATUS, &status));
	if (status != GL_TRUE)
	{
		std::cout << "Error in Validation" << std::endl;
	}

	// clean up the intermediate files 
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1fv(const std::string& name, const float value)
{
	GLCall(glUniform1fv(GetUniformLocation(name), 1, &value));
}

void Shader::SetUniform3fv(const std::string& name, const glm::vec3& value)
{
	GLCall(glUniform3fv(GetUniformLocation(name), 1, &value[0]));
}

void Shader::SetUniform4fv(const std::string& name, const glm::vec4& value)
{
	GLCall(glUniform4fv(GetUniformLocation(name), 1, &value[0]));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}
	m_UniformLocationCache[name] = location;
	return location;
}
