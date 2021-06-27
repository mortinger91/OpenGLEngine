#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "vendorLibs.h"

#include "tests/TestClearColor.h"
#include "tests/Test3DCube.h"

#define WIDTH 1920
#define HEIGHT 1080

// opengl old mode
void TestFunction()
{
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f( 0.0f,  0.5f);
	glVertex2f( 0.5f, -0.5f);
	glEnd();
}

void WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	//glfwSetWindowSize(window, width, height);
	GLCall(glViewport(0, 0, width, height));
}

void ImGuiInit(GLFWwindow*& window)
{
	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
}

int Init(GLFWwindow*& window)
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
    window = glfwCreateWindow(WIDTH, HEIGHT, "An OpenGL Viewport (GLFW)", NULL, NULL);
    if (!window)
    {
		std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	// setup the callback called when the window is resized
	glfwSetWindowSizeCallback(window, WindowResizeCallback);

	glfwSwapInterval(1);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  //fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	  std::cerr << "Error: " << glewGetString(err) << std::endl; 
	}

	GLCall(const unsigned char* version = glGetString(GL_VERSION));
	std::cout << std::string(reinterpret_cast<const char*>(version)) << std::endl;

	ImGuiInit(window);

	return 0;
}

void processInput(GLFWwindow* window)
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
		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::Test3DCube>("3D Cube");

		Renderer renderer;

		// Game engine loop, do it every frame
		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window))
		{
			GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
			// input processing done by glfw
			processInput(window);
			// Clearing last frame
			renderer.Clear();

			//New Frame ImGui
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			if(currentTest)
			{
				currentTest->OnUpdate(0.f);
				currentTest->OnRender(window, width, height);
				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("<-"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}

			//Render ImGui
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Swap front and back buffers
			// The glfwSwapBuffers will swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window) that is used to render to during this render iteration and show it as output to the screen.
			glfwSwapBuffers(window);

			// Poll for and process events 
			// The glfwPollEvents function checks if any events are triggered (like keyboard input or mouse movement events), updates the window state, and calls the corresponding functions (which we can register via callback methods)
			glfwPollEvents();
		}
		delete currentTest;
		if (currentTest != testMenu)
		{
			delete testMenu;
		}
	}

	// Shutdown ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	// Destroy GLFW
    glfwTerminate();

	return 0;
}