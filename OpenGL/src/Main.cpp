#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
		std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	std::cout << glGetString(GL_VERSION) << std::endl;
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
	{
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

		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		// creating an index buffer
		IndexBuffer ib(indices, 6);
	
		// defining a shader
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		// defining a uniform after having bound a shader
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		// unbiding all 
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		// state setting function
		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

		Renderer renderer;

		float r = 0.f;
		float increment = 0.05f;

		// Game engine loop, do it every frame
		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window))
		{
			processInput(window);

			// Render here
			renderer.Clear();

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);

			if (r > 1.f)
				increment = -0.05f;
			else if (r < 0.f)
				increment = 0.05f;

			r += increment;

			// Swap front and back buffers
			// The glfwSwapBuffers will swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window) that is used to render to during this render iteration and show it as output to the screen.
			glfwSwapBuffers(window);

			// Poll for and process events 
			// The glfwPollEvents function checks if any events are triggered (like keyboard input or mouse movement events), updates the window state, and calls the corresponding functions (which we can register via callback methods)
			glfwPollEvents();
		}
	}
    glfwTerminate();
    return 0;
}