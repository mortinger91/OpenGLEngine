#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	// creates an empty shader object and returns a non-zero value by which it can be referenced
	// A shader object is used to maintain the source code strings that define a shader.
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		 int lenght;
		 glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		 char* message = (char*)alloca(lenght * sizeof(char));
		 glGetShaderInfoLog(id, lenght, &lenght, message);
		 cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << endl;
		 cout << message << endl;
		 glDeleteShader(id);
		 return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// Creates a program object
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	// Specifies the handle of the program object to be validated
	glValidateProgram(program);

	// check if the program is correct
	int status;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if (status != GL_TRUE)
	{
		cout << "Error in Validation" << endl;
	}

	// clean up the intermediate files 
	glDeleteShader(vs);
	glDeleteShader(fs);

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

	float positions[6] = 
	{
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	// creating a buffer
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	// binding a buffer, selecting it
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// creates and initializes a buffer object's data store
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	// abilitare un attributo per i vertici nel buffer
	glEnableVertexAttribArray(0);
	// setup dell'unico attributo "position" del buffer attualmente bindato
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
	
	// creating some shaders
	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = position;\n"
		"}\n";

	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";
	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	// state setting function
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Game engine loop, do it every frame
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
		processInput(window);

        /* Render here */
		// state using function
        glClear(GL_COLOR_BUFFER_BIT);

		


		// DRAW CALL: render primitives from array data
		glDrawArrays(GL_TRIANGLES, 0, 3);




        /* Swap front and back buffers */
		// The glfwSwapBuffers will swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window) that is used to render to during this render iteration and show it as output to the screen.
        glfwSwapBuffers(window);

        /* Poll for and process events */
		// The glfwPollEvents function checks if any events are triggered (like keyboard input or mouse movement events), updates the window state, and calls the corresponding functions (which we can register via callback methods)
        glfwPollEvents();
    }

	glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}