#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// il __ fa capire che questa funzione è specifica del compilatore, msvc in questo caso
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

using std::cout;
using std::endl;

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	GLenum error;
	while (error = glGetError())
	{
		cout << "OpenGL Error: (" <<  error << ") " <<
			function << " " << file << ":" << line <<endl;
		return false;
	}
	return true;
}

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		// std::string::npos è una invalid string position, find ritorna la posizione della stringa
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
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
		 cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << endl;
		 cout << message << endl;
		 GLCall(glDeleteShader(id));
		 return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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
		cout << "Error in Validation" << endl;
	}

	// clean up the intermediate files 
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

// opengl old mode
void TestFunction()
{
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f( 0.0f,  0.5f);
	glVertex2f( 0.5f, -0.5f);
	glEnd();
}

int Init(GLFWwindow* &window)
{
	/* Initialize the library */
    if (!glfwInit())
	{
		return -1;
	}

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
		cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	cout << glGetString(GL_VERSION) << endl;
	return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(void)
{
	GLFWwindow* window = nullptr;
    if (Init(window) != 0)
	{
		return -1;
	}

	float positions[] = 
	{
		-0.5f, -0.5f, //0
		 0.5f, -0.5f, //1
		 0.5f,  0.5f, //2
		-0.5f,  0.5f, //3
	};

	unsigned int indices[] = 
	{
		0, 1, 2,
		2, 3, 0
	};

	// creating a vertex buffer
	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	// binding a vertex buffer object (selecting it)
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	// creates and initializes a buffer object's data store
	GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

	// setup dell'unico attributo "position" del buffer attualmente bindato
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0));
	// abilitare un attributo per i vertici nel buffer
	GLCall(glEnableVertexAttribArray(0));

	// creating an index buffer
	unsigned int ibo;
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));
	
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	// state setting function
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	// Game engine loop, do it every frame
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
		processInput(window);

        // Render here
		// state using function
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

		


		// DRAW CALL: render primitives from array data
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));



        // Swap front and back buffers
		// The glfwSwapBuffers will swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window) that is used to render to during this render iteration and show it as output to the screen.
        glfwSwapBuffers(window);

        // Poll for and process events 
		// The glfwPollEvents function checks if any events are triggered (like keyboard input or mouse movement events), updates the window state, and calls the corresponding functions (which we can register via callback methods)
        glfwPollEvents();
    }

	GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}