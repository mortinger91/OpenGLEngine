#include "Test3DCube.h"

#include <chrono>
#include "Shader.h"
#include "meshes/cube.h"
#include "meshes/plane.h"
#include "Utility.h"
#include "ModelLoader.h"

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
		  m_PointLightAngle(0)
	{
		GLCall(glEnable(GL_BLEND));
		// setting up a blend function, default would be src=0 dest=1 which means override old pixels with new ones
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));
		
		std::string resPath;
		Utility::GetPathToRes(resPath);

		// Init of all the elements in the Test3DCube scene
		// defining a shader
		m_Shaders["default"] = std::make_shared<Shader>(resPath + "/shaders/Basic.vert", resPath + "/shaders/Basic.frag");

		// creating the materials
		float ambient = 0.15f;
		m_Materials["cubeuv"] = std::make_shared<Material>(resPath + "/textures/cubeUV.png", 500.f, 0.75f, 0.2f, ambient, m_Shaders["default"]);
		m_Materials["teapot"] = std::make_shared<Material>(resPath + "/textures/metal.png", 200.f, 5.f, 5.f, ambient, m_Shaders["default"]);
		m_Materials["plane"] = std::make_shared<Material>(resPath + "/textures/metal.png", 500.f, 0.75f, 0.2f, ambient, m_Shaders["default"]);
		m_Materials["white"] = std::make_shared<Material>(resPath + "/textures/white.png", 1.f, 1.f, 1.f, 300.f, m_Shaders["default"]);

		// creating the camera
		m_Camera = std::make_unique<Camera>(glm::vec3(0.f, 0.f, 60.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

		// creating the models
		Model model1("Cube");
		auto mesh1 = std::make_unique<Mesh>("Cube", Cube::positions, 
											//Cube::colors, 
											Cube::normals, Cube::texturesUVs, Cube::indices);
		mesh1->SetMaterial(m_Materials["cubeuv"]);
		mesh1->m_Scale = 6.f;
		model1.MoveMesh(std::move(mesh1));
		model1.m_TranslationVec = glm::vec3(-12.f, 0.f, 0.f);
		m_Models.push_back(std::move(model1));

		Model model2("Teapot");
		auto mesh2 = std::make_unique<Mesh>("Teapot", resPath+"/meshes/teapot.obj");
		mesh2->SetMaterial(m_Materials["teapot"]);
		mesh2->m_Scale = 12.f;
		model2.MoveMesh(std::move(mesh2));
		model2.m_TranslationVec = glm::vec3(7.f, 0.f, 0.f);
		m_Models.push_back(std::move(model2));

		Model model2b("Teapot");
		ModelLoader::LoadModel(model2b, resPath + "/meshes/teapot.obj", m_Materials["teapot"]);
		model2b.m_Scale = 6.f;
		model2b.m_TranslationVec = glm::vec3(0.f, 0.f, 0.f);
		m_Models.push_back(std::move(model2b));

		Model model3("Plane");
		auto mesh3 = std::make_unique<Mesh>("Plane", Plane::positions, 
											//Plane::colors, 
											Plane::normals, Plane::textures, Plane::indices);
		mesh3->SetMaterial(m_Materials["plane"]);
		mesh3->m_Scale = 500.f;
		model3.MoveMesh(std::move(mesh3));
		model3.m_TranslationVec = glm::vec3(0.f, -10.f, 0.f);
		m_Models.push_back(std::move(model3));

		Model model4("PointLightCube");
		auto mesh4 = std::make_unique<Mesh>("PointLightCube", Cube::positions, 
											//Cube::colors, 
											Cube::normals, Cube::texturesUnique, Cube::indices);
		mesh4->SetMaterial(m_Materials["white"]);
		model4.MoveMesh(std::move(mesh4));
		model4.m_TranslationVec = glm::vec3(30.f, 0.f, 0.f);
		m_Models.push_back(std::move(model4));

		Model model5("Car");
		ModelLoader::LoadModel(model5, resPath+"/meshes/car.obj", m_Materials["teapot"]);
		model5.m_TranslationVec = glm::vec3(15.f, 0.f, 0.f);
		m_Models.push_back(Model("Car_0", model5));
		m_Models.push_back(Model("Car_1", model5));
		m_Models[m_Models.size()-1].m_TranslationVec = glm::vec3(-15.f, 0.f, 0.f);

		std::cout << "Finished loading models" << std::endl;
	}

	void Test3DCube::OnRender(GLFWwindow *window, int width, int height)
	{
		GLCall(glClearColor(0.2f, 0.3f, 0.8f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));

		// TODO: Move inputs to a input class
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_Camera->RotateViewHorizontal(1.f);
		}
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_Camera->RotateViewHorizontal(-1.f);
		}
			
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_Camera->RotateViewVertical(1.f);
		}
		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_Camera->RotateViewVertical(-1.f);
		}

		{
			// TODO: cache these matrix and modify them only if the values are changed
			// View matrix, common for all models and meshes
			m_Camera->CreateViewMatrix();
			
			glm::vec4 light0direction;
			light0direction = m_Camera->m_ViewMatrix * light_direction;
			glm::vec4 light1position;
			if (m_RotatingPointLight)
			{
				m_PointLightAngle+=0.1f;
			}
			glm::mat4 matRotateLight;
			Utility::NormalizeAngle(m_PointLightAngle);
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

			if (!m_StopRotation)
			{
				m_Models[0].m_RotationVec.y+=0.2f;
				Utility::NormalizeAngle(m_Models[0].m_RotationVec.y);
				m_Models[1].m_RotationVec.y-=0.3f;
				Utility::NormalizeAngle(m_Models[1].m_RotationVec.y);
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
		ImGui::SliderFloat("Translate X1", &m_Models[0].m_TranslationVec.x, -100.f, 100.f);
		ImGui::SliderFloat("Translate Y1", &m_Models[0].m_TranslationVec.y, -100.f, 100.f);
		ImGui::SliderFloat("Translate Z1", &m_Models[0].m_TranslationVec.z, -1000.f, 1000.f);
		ImGui::SliderFloat3("Model Rotation1", &m_Models[0].m_RotationVec.x, 0.f, 360.f);
		ImGui::SliderFloat("Scale1", &m_Models[0].m_Scale, 0.f, 50.f);

		ImGui::SliderFloat("Translate X2", &m_Models[1].m_TranslationVec.x, -100.f, 100.f);
		ImGui::SliderFloat("Translate Y2", &m_Models[1].m_TranslationVec.y, -100.f, 100.f);
		ImGui::SliderFloat("Translate Z2", &m_Models[1].m_TranslationVec.z, -1000.f, 1000.f);
		ImGui::SliderFloat3("Model Rotation2", &m_Models[1].m_RotationVec.x, 0.f, 360.f);
		ImGui::SliderFloat("Scale2", &m_Models[1].m_Scale, 0.f, 50.f);

		ImGui::SliderFloat("nearPlane", &m_NearPlane, -50.f, 50.f);
		ImGui::SliderFloat("farPlane", &m_FarPlane, 0.f, 1000.f);
		ImGui::SliderFloat("fovy", &m_Fov, 0.f, 90.f);

		ImGui::SliderFloat("light_color0R", &light_color[0], 0.f, 20.f);
		ImGui::SliderFloat("light_color0G", &light_color[1], 0.f, 20.f);
		ImGui::SliderFloat("light_color0B", &light_color[2], 0.f, 20.f);

		ImGui::SliderFloat("light_color1R", &light_color1[0], 0.f, 20.f);
		ImGui::SliderFloat("light_color1G", &light_color1[1], 0.f, 20.f);
		ImGui::SliderFloat("light_color1B", &light_color1[2], 0.f, 20.f);

		if(ImGui::Button("change proj matrix"))
		{
			m_UseOrtho = !m_UseOrtho;
		}
		if (m_UseOrtho)
			ImGui::Text("Using Ortho");
		else
			ImGui::Text("Using Perspective");
		if(ImGui::Button("enable/disable light"))
		{
			m_IsLight = (m_IsLight == 0) ? 1 : 0;
		}
		if (m_IsLight == 1)
			ImGui::Text("Using Light");
		else
			ImGui::Text("Not Using Light");
		if(ImGui::Button("stop rotation"))
		{
			m_StopRotation = !m_StopRotation;
		}
		if (m_StopRotation)
			ImGui::Text("Rotation Stopped");
		else
			ImGui::Text("Rotation Going");
		if(ImGui::Button("rotating point light"))
		{
			m_RotatingPointLight = !m_RotatingPointLight;
		}
		if (m_RotatingPointLight)
			ImGui::Text("Rotation Point light Going");
		else
			ImGui::Text("Rotation Point light Stopped");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	Test3DCube::~Test3DCube()
	{
	}

	void Test3DCube::OnUpdate(float deltaTime)
	{
	}

}