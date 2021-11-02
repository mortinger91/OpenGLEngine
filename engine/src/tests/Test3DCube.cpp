#include "Test3DCube.h"

#include <chrono>
#include <future>
#include <thread>
#include "Shader.h"
#include "meshes/cube.h"
#include "meshes/plane.h"
#include "Utility.h"
#include "ModelLoader.h"
#include "Channel.h"

namespace test
{
	Test3DCube::Test3DCube()
		: m_NearPlane(1.f),
		  m_FarPlane(1000.f),
		  m_UseOrtho(false),
		  m_Fov(30.0f),
		  m_IsLight(1),
		  m_StopRotation(true),
		  m_RotatingPointLight(true),
		  m_PointLightAngle(0),
		  m_SelectedModel(0)
	{
		GLCall(glEnable(GL_BLEND));
		// setting up a blend function, default would be src=0 dest=1 which means override old pixels with new ones
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));
		
		Utility::GetPathToRes(m_ResPath);

		// Init of all the elements in the Test3DCube scene
		// defining a shader
		m_Shaders["default"] = std::make_shared<Shader>(m_ResPath + "/shaders/Basic.vert", m_ResPath + "/shaders/Basic.frag");
		
		// TODO: move the loading of the materials to the loading mesh thread, so that the window and the interface is responsive as soon as possible
		// creating the materials
		#ifdef DEBUG
			long double start;
			long double passed;
			start = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		#endif
		float ambient = 0.15f;
		m_Materials["cubeuv"] = std::make_shared<Material>(m_ResPath + "/textures/cubeUV.png", 500.f, 0.75f, 0.2f, ambient, m_Shaders["default"]);
		m_Materials["teapot"] = std::make_shared<Material>(m_ResPath + "/textures/metal.png", 200.f, 5.f, 5.f, ambient, m_Shaders["default"]);
		m_Materials["plane"] = std::make_shared<Material>(m_ResPath + "/textures/metal.png", 500.f, 0.75f, 0.2f, ambient, m_Shaders["default"]);
		m_Materials["white"] = std::make_shared<Material>(m_ResPath + "/textures/white.png", 1.f, 1.f, 1.f, 300.f, m_Shaders["default"]);
		m_Materials["medkit_base"] = std::make_shared<Material>(m_ResPath + "/textures/medkit_BaseColor.png", 1.f, 1.f, 1.f, 1.f, m_Shaders["default"]);
		m_Materials["medkit_light_base"] = std::make_shared<Material>(m_ResPath + "/textures/medkit_Light_BaseColor.png", 1.f, 1.f, 1.f, 1.f, m_Shaders["default"]);
		#ifdef DEBUG
			passed = (std::chrono::high_resolution_clock::now().time_since_epoch().count() - start) / 1000000;
			std::cout << REDTEXT("Materials loaded in: " + std::to_string(passed) + " ms\n");
		#endif

		// loading meshes async
		m_LoadingMeshesThread = new std::thread(&Test3DCube::LoadModels, this);

		// creating the camera
		m_Camera = std::make_unique<Camera>(glm::vec3(0.f, 0.f, 60.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		m_Camera->CreateViewMatrix();
	}

	bool Test3DCube::LoadModels()
	{
		// creating the models
		#ifdef DEBUG
			long double start;
			long double passed;
			start = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		#endif
		Model model1("Cube");
		auto mesh1 = std::make_shared<Mesh>("Cube", Cube::positions, Cube::normals, Cube::texturesUVs, Cube::indices);
		mesh1->SetMaterial(m_Materials["cubeuv"]);
		mesh1->m_Scale = 6.f;
		Channels::meshChan.put(mesh1);
		Channels::toRead++;
		model1.MoveMesh(std::move(mesh1));
		model1.m_TranslationVec = glm::vec3(0.f, 0.f, -20.f);
		m_Models.push_back(std::move(model1));
		
		Model model2b("Teapot");
		ModelLoader::LoadModel(model2b, m_ResPath + "/meshes/teapot.obj", { m_Materials["teapot"] }, { 0 });
		model2b.m_Scale = 6.f;
		model2b.m_TranslationVec = glm::vec3(0.f, 0.f, 0.f);
		m_Models.push_back(std::move(model2b));

		Model model3("Plane");
		auto mesh3 = std::make_shared<Mesh>("Plane", Plane::positions, Plane::normals, Plane::textures, Plane::indices);
		mesh3->SetMaterial(m_Materials["plane"]);
		mesh3->m_Scale = 500.f;
		Channels::meshChan.put(mesh3);
		Channels::toRead++;
		model3.MoveMesh(std::move(mesh3));
		model3.m_TranslationVec = glm::vec3(0.f, -10.f, 0.f);
		m_Models.push_back(std::move(model3));

		Model model4("PointLightCube");
		auto mesh4 = std::make_shared<Mesh>("PointLightCube", Cube::positions, Cube::normals, Cube::texturesUnique, Cube::indices);
		mesh4->SetMaterial(m_Materials["white"]);
		Channels::meshChan.put(mesh4);
		Channels::toRead++;
		model4.MoveMesh(std::move(mesh4));
		model4.m_TranslationVec = glm::vec3(30.f, 0.f, 0.f);
		m_Models.push_back(std::move(model4));
		
		Model model5("Car");
		ModelLoader::LoadModel(model5, m_ResPath + "/meshes/car.obj", { m_Materials["teapot"],m_Materials["white"] }, { 0,1,0,0,0,1,1,0,0,0,0 });
		model5.m_UseTextures = false;

		m_Models.push_back(Model("Car_0", model5));
		m_Models[m_Models.size() - 1].m_TranslationVec = glm::vec3(20.f, 0.f, 0.f);
		m_Models.push_back(Model("Car_1", model5));
		m_Models[m_Models.size() - 1].m_TranslationVec = glm::vec3(-20.f, 0.f, 0.f);

		Model model6("Medkit");
		ModelLoader::LoadModel(model6, m_ResPath + "/meshes/medkit.fbx", { m_Materials["medkit_base"], m_Materials["medkit_light_base"] }, { 1, 0 });
		m_Models.push_back(Model("Medkit", model6));
		m_Models[m_Models.size() - 1].m_TranslationVec = glm::vec3(0.f, 0.f, 10.f);
		m_Models[m_Models.size() - 1].m_Scale = 10.f;
		m_Models[m_Models.size() - 1].m_RotationVec.x = 270.f;

		std::cout << "Finished loading models" << std::endl;
		#ifdef DEBUG
			passed = (std::chrono::high_resolution_clock::now().time_since_epoch().count() - start) / 1000000;
			std::cout << REDTEXT("All scene meshes loaded in: " + std::to_string(passed) + " ms\n");
		#endif
		
		return true;
	}

	// TODO: Add zoom in and out using mouse wheel
	//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	//{
	//	
	//}

	void Test3DCube::OnRender(GLFWwindow *window, int width, int height)
	{
		GLCall(glClearColor(0.2f, 0.3f, 0.8f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));

		bool cameraMoved = false;
		// TODO: Move inputs to a input class
		// TODO: Change camera rotation
		// TODO: Add zoom in, zoom out using scrolling wheel
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_Camera->RotateViewHorizontal(1.f);
			cameraMoved = true;
		}
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_Camera->RotateViewHorizontal(-1.f);
			cameraMoved = true;
		}
			
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_Camera->RotateViewVertical(1.f);
			cameraMoved = true;
		}
		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_Camera->RotateViewVertical(-1.f);
			cameraMoved = true;
		}

		//glfwSetScrollCallback(window, scroll_callback);

		{
			// TODO: cache these matrix and modify them only if the values are changed
			// View matrix, common for all models and meshes
			if (cameraMoved)
			{
				m_Camera->CreateViewMatrix();
				cameraMoved = false;
			}
			
			glm::vec4 light0direction;
			light0direction = m_Camera->m_ViewMatrix * light_direction;
			glm::vec4 light1position;
			if (m_RotatingPointLight)
			{
				m_PointLightAngle+=0.1f;
				Utility::NormalizeAngle(m_PointLightAngle);
			}
			glm::mat4 matRotateLight;
			Utility::CreateRotationGenericMatrix(matRotateLight, m_PointLightAngle, glm::vec3(0.f, 1.f, 0.f));
			light1position = m_Camera->m_ViewMatrix * matRotateLight * light_position1;

			// Perspective matrix, common for all models and meshes
			if (m_UseOrtho)
			{
				m_ProjMatrix = Utility::CreateOrthoMatrix((float)width, (float)height, m_NearPlane, m_FarPlane);
			}
			else
			{
				m_ProjMatrix = Utility::CreatePerspectiveMatrix(m_Fov, (float)width / (float)height, m_NearPlane, m_FarPlane);
			}

			m_Shaders["default"]->Bind();
			// Uniforms regarding point and directional lights
			m_Shaders["default"]->SetUniform1i("islight", m_IsLight);
			m_Shaders["default"]->SetUniform3fv("light0dirn", light0direction);
			m_Shaders["default"]->SetUniform4fv("light0color", light_color);
			m_Shaders["default"]->SetUniform4fv("light1posn", light1position);
			m_Shaders["default"]->SetUniform4fv("light1color", light_color1);
			// TODO: do i need to pass eyepos to fragment shader for correct highlights?
			//m_Shader->SetUniform3fv("eyepos", m_Camera->GetEyePos());


			if (!m_StopRotation && m_Models.size() > 1)
			{
				m_Models[0].m_RotationVec.y+=0.2f;
				Utility::NormalizeAngle(m_Models[0].m_RotationVec.y);
				m_Models[1].m_RotationVec.y-=0.3f;
				Utility::NormalizeAngle(m_Models[1].m_RotationVec.y);
			}

			//check if a new model is loaded so its vertex array can be constructed
			
			while (Channels::toRead > 0)
			{
				std::shared_ptr<Mesh> m = Channels::meshChan.get();
				m->MakeVertexArray();
				Channels::toRead--;
			}

			// draw each model
			for(int i = 0; i < m_Models.size(); ++i)
			{
				// only for point light mesh
				if (m_Models[i].GetName() == "PointLightCube")
				{
					m_Models[i].m_Meshes[0]->m_CustomTransform = matRotateLight;
				}

				m_Models[i].Draw(m_Camera->m_ViewMatrix, m_ProjMatrix);
			}
		}
	}

	void Test3DCube::OnImGuiRender()
	{
		if (m_Models.size() > 0)
		{
			ImGui::SliderFloat("Translate X", &m_Models[m_SelectedModel].m_TranslationVec.x, -100.f, 100.f);
			ImGui::SliderFloat("Translate Y", &m_Models[m_SelectedModel].m_TranslationVec.y, -100.f, 100.f);
			ImGui::SliderFloat("Translate Z", &m_Models[m_SelectedModel].m_TranslationVec.z, -1000.f, 1000.f);
			ImGui::SliderFloat3("Model Rotation", &m_Models[m_SelectedModel].m_RotationVec.x, 0.f, 360.f);
			ImGui::SliderFloat("Scale", &m_Models[m_SelectedModel].m_Scale, 0.f, 50.f);

			ImGui::SliderFloat("shininess1", &m_Models[m_SelectedModel].m_Meshes[0]->m_Material->m_Shininess, 1.f, 1000.f);
			ImGui::SliderFloat("specular1", &m_Models[m_SelectedModel].m_Meshes[0]->m_Material->m_Specular, 1.f, 100.f);
			ImGui::SliderFloat("diffuse1", &m_Models[m_SelectedModel].m_Meshes[0]->m_Material->m_Diffuse, 1.f, 100.f);
			ImGui::SliderFloat("ambient1", &m_Models[m_SelectedModel].m_Meshes[0]->m_Material->m_Ambient, 1.f, 100.f);

			if (ImGui::Button(("Selected Model: " + m_Models[m_SelectedModel].GetName()).c_str()))
			{
				m_SelectedModel = m_SelectedModel >= m_Models.size() - 1 ? 0 : m_SelectedModel + 1;
			}
			std::string usingTex = m_Models[m_SelectedModel].m_UseTextures ? std::string("Yes") : std::string("No");
			if (ImGui::Button(("Use Textures: " + usingTex).c_str()))
			{
				m_Models[m_SelectedModel].m_UseTextures = !m_Models[m_SelectedModel].m_UseTextures;
			}
		}

		std::string projString = m_UseOrtho ? std::string("Ortho") : std::string("Perspective");
		if (ImGui::Button(("Using: " + projString + " Projection matrix").c_str()))
		{
			m_UseOrtho = !m_UseOrtho;
		}
		
		std::string lightString = m_IsLight ? std::string("Enabled") : std::string("Disabled");
		if(ImGui::Button(("Lights: " + lightString).c_str()))
		{
			m_IsLight = (m_IsLight == 0) ? 1 : 0;
		}

		std::string rotEnabled = m_StopRotation ? std::string("Stopped") : std::string("Going");
		if(ImGui::Button( ("Cube and Teapot rotation: " + rotEnabled).c_str() ))
		{
			m_StopRotation = !m_StopRotation;
		}

		std::string rotationTex = m_RotatingPointLight ? std::string("Going") : std::string("Stopped");
		if(ImGui::Button(("Rotation Point light: " + rotationTex).c_str()))
		{
			m_RotatingPointLight = !m_RotatingPointLight;
		}

		ImGui::SliderFloat("light_color0R", &light_color[0], 0.f, 20.f);
		ImGui::SliderFloat("light_color0G", &light_color[1], 0.f, 20.f);
		ImGui::SliderFloat("light_color0B", &light_color[2], 0.f, 20.f);

		ImGui::SliderFloat("light_color1R", &light_color1[0], 0.f, 20.f);
		ImGui::SliderFloat("light_color1G", &light_color1[1], 0.f, 20.f);
		ImGui::SliderFloat("light_color1B", &light_color1[2], 0.f, 20.f);

		ImGui::SliderFloat("fovy", &m_Fov, 0.f, 90.f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	Test3DCube::~Test3DCube()
	{
		m_LoadingMeshesThread->join();
	}

	void Test3DCube::OnUpdate(float deltaTime)
	{
	}

}
