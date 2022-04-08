#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "vendorLibs.h"

#include "tests/TestClearColor.h"
#include "tests/Test3DCube.h"

#define WIDTH 1920
#define HEIGHT 1080

void WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	//glfwSetWindowSize(window, width, height);
	GLCall(glViewport(0, 0, width, height));
}

void ImGuiInit(GLFWwindow*& window)
{
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void ImGuiDocking()
{
	static bool opt_fullscreen = false;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into, because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// We proceed even if Begin() returns false (aka window is collapsed). This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, all active windows docked into it will lose their parent and become undocked. We cannot preserve the docking relationship between an active window and an inactive docking, otherwise any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("ImGui Dockable Window", nullptr, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
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

int openGLEngine()
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

		// adding specific tests to the menu
		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::Test3DCube>("3D Cube");
		std::cout << std::endl;

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

			// New Frame ImGui
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			if(currentTest)
			{
				// actual rendering of the selected test
				currentTest->OnUpdate(0.f);
				currentTest->ProcessInput(window, width, height);
				currentTest->OnRender();
				ImGuiDocking();
				ImGui::Begin("ImGui Panel 1");
				if (currentTest != testMenu && ImGui::Button("<-"))
				{
					// calling the selected test destructor
					std::cout << "Returning to selection menu\n\n";
					delete currentTest;
					currentTest = testMenu;
				}

				currentTest->OnImGuiRender();
			
				ImGui::End();
				ImGui::End();
			}

			//Render ImGui
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			
			// Update and Render additional Platform Windows
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

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
	glfwDestroyWindow(window);
    glfwTerminate();

	return 0;
}
