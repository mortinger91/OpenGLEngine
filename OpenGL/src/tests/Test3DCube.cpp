#include "Test3DCube.h"

#include "Renderer.h"
#include "imgui/imgui.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "cube.h"
#include "plane.h"
#include "Utility.h"
#include <chrono>

namespace test
{
	Test3DCube::Test3DCube()
		: m_NearPlane(1.f),
		  m_FarPlane(1000.f),
		  m_UseOrtho(false),
		  m_Fov(30.0f),
		  m_IsLight(1),
		  m_UseTexture(1),
		  index(0),
		  m_StopRotation(false),
		  m_RotatingPointLight(true),
		  m_PointLightAngle(0)
	{
		GLCall(glEnable(GL_BLEND));
		// setting up a blend function, default would be src=0 dest=1 which means override old pixels with new ones
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));
		
		// defining a shader
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.vert", "res/shaders/Basic.frag");
		m_Shader->Bind();

		// creating the camera
		m_Camera = std::make_unique<Camera>(glm::vec3(0.f, 0.f, 60.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

		m_Shader->SetUniform1i("u_Texture", 0);

		float ambient = 0.15f;

		Mesh mesh1("Cube", Cube::positions, Cube::colors, Cube::normals, Cube::textures, Cube::indices);
		mesh1.SetMaterial("res/textures/metal.png", 500.f, 0.75f, 0.2f, ambient);
		mesh1.m_Scale = 6.f;
		mesh1.m_TranslationVec = glm::vec3(-12.f, 0.f, 0.f);
		m_MeshVector.push_back(std::move(mesh1));
		Mesh mesh2("Teapot", "res/meshes/teapot.obj");
		mesh2.SetMaterial("res/textures/metal.png", 200.f, 0.25f, 0.8f, ambient);
		mesh2.m_Scale = 12.f;
		mesh2.m_TranslationVec = glm::vec3(7.f, 0.f, 0.f);
		m_MeshVector.push_back(std::move(mesh2));
		Mesh mesh3("Plane", Plane::positions, Plane::colors, Plane::normals, Plane::textures, Plane::indices);
		mesh3.SetMaterial("res/textures/metal.png", 500.f, 0.75f, 0.2f, ambient);
		mesh3.m_Scale = 500.f;
		mesh3.m_TranslationVec = glm::vec3(0.f, -10.f, 0.f);
		m_MeshVector.push_back(std::move(mesh3));
	}

	void Test3DCube::OnRender(GLFWwindow *window, int width, int height)
	{
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

		GLCall(glClearColor(0.2f, 0.3f, 0.8f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));

		{
			m_Camera->CreateViewMatrix();

			if (!m_StopRotation)
			{
				m_MeshVector[0].m_RotationVec.y+=0.2f;
				m_MeshVector[1].m_RotationVec.y-=0.3f;
			}

			for(int i = 0; i < m_MeshVector.size(); ++i)
			{
				glm::mat4 mv;
				glm::mat4 mvp;

				Utility::NormalizeAngle(m_MeshVector[i].m_RotationVec.y);
				m_MeshVector[i].CreateModelMatrix();
				mv = m_Camera->m_ViewMatrix * m_MeshVector[i].m_ModelMatrix;

				glm::vec4 light0direction;
				light0direction = m_Camera->m_ViewMatrix * light_direction;

				glm::vec4 light1position;
				if (m_RotatingPointLight)
				{
					m_PointLightAngle+=0.2f;
				}
				glm::mat4 matRotateLight;
				Utility::NormalizeAngle(m_PointLightAngle);
				Utility::CreateRotationGenericMatrix(matRotateLight, m_PointLightAngle, glm::vec3(0.f, 1.f, 0.f));
				light1position = m_Camera->m_ViewMatrix * matRotateLight * light_position1;
				
				// TODO: modify these matrix only if the values are changed
				if(m_UseOrtho)
				{
					m_ProjOrtho = Utility::CreateOrthoMatrix((float)width, (float)height, m_NearPlane, m_FarPlane);
					mvp = m_ProjOrtho * mv;
				}
				else
				{
					m_ProjPersp = Utility::CreatePerspectiveMatrix(m_Fov, (float)width / (float)height, m_NearPlane, m_FarPlane);
					mvp = m_ProjPersp * mv;
				}

				m_Shader->SetUniformMat4f("u_MV", mv);
				m_Shader->SetUniformMat4f("u_MVP", mvp);
				m_Shader->SetUniform1i("useTexture", m_UseTexture);
				m_Shader->SetUniform1i("islight", m_IsLight);
				m_Shader->SetUniform3fv("light0dirn", light0direction);
				m_Shader->SetUniform4fv("light0color", light_color);
				m_Shader->SetUniform4fv("light1posn", light1position);
				m_Shader->SetUniform4fv("light1color", light_color1);
				m_Shader->SetUniform4fv("ambient", { m_MeshVector[i].m_Material->m_Ambient, m_MeshVector[i].m_Material->m_Ambient, m_MeshVector[i].m_Material->m_Ambient, 1 });
				m_Shader->SetUniform4fv("diffuse", { m_MeshVector[i].m_Material->m_Diffuse, m_MeshVector[i].m_Material->m_Diffuse, m_MeshVector[i].m_Material->m_Diffuse, 1 });
				m_Shader->SetUniform4fv("specular", { m_MeshVector[i].m_Material->m_Specular, m_MeshVector[i].m_Material->m_Specular, m_MeshVector[i].m_Material->m_Specular, 1 });
				m_Shader->SetUniform1fv("shininess", m_MeshVector[i].m_Material->m_Shininess);

				// TODO: is eyepos needed in fragment shader for correct highlights?
				//m_Shader->SetUniform3fv("eyepos", m_Camera->GetEyePos());

				renderer.Draw(m_MeshVector[i], *m_Shader);
			}
		}
	}

	void Test3DCube::OnImGuiRender()
	{
		ImGui::SliderFloat("Translate X1", &m_MeshVector[0].m_TranslationVec.x, -100.f, 100.f);
		ImGui::SliderFloat("Translate Y1", &m_MeshVector[0].m_TranslationVec.y, -100.f, 100.f);
		ImGui::SliderFloat("Translate Z1", &m_MeshVector[0].m_TranslationVec.z, -1000.f, 1000.f);
		ImGui::SliderFloat3("Model Rotation1", &m_MeshVector[0].m_RotationVec.x, 0.f, 360.f);
		ImGui::SliderFloat("Scale1", &m_MeshVector[0].m_Scale, 0.f, 50.f);

		ImGui::SliderFloat("Translate X2", &m_MeshVector[1].m_TranslationVec.x, -100.f, 100.f);
		ImGui::SliderFloat("Translate Y2", &m_MeshVector[1].m_TranslationVec.y, -100.f, 100.f);
		ImGui::SliderFloat("Translate Z2", &m_MeshVector[1].m_TranslationVec.z, -1000.f, 1000.f);
		ImGui::SliderFloat3("Model Rotation2", &m_MeshVector[1].m_RotationVec.x, 0.f, 360.f);
		ImGui::SliderFloat("Scale2", &m_MeshVector[1].m_Scale, 0.f, 50.f);

		ImGui::SliderFloat("nearPlane", &m_NearPlane, -50.f, 50.f);
		ImGui::SliderFloat("farPlane", &m_FarPlane, 0.f, 1000.f);
		ImGui::SliderFloat("fovy", &m_Fov, 0.f, 90.f);
		ImGui::SliderFloat("shininess1", &m_MeshVector[0].m_Material->m_Shininess, 0.f, 1000.f); 
		ImGui::SliderFloat("specular1", &m_MeshVector[0].m_Material->m_Specular, 0.f, 1.f); 
		ImGui::SliderFloat("diffuse1", &m_MeshVector[0].m_Material->m_Diffuse, 0.f, 1.f); 
		ImGui::SliderFloat("ambient1", &m_MeshVector[0].m_Material->m_Ambient, 0.f, 1.f);
		ImGui::SliderFloat("shininess2", &m_MeshVector[1].m_Material->m_Shininess, 0.f, 1000.f); 
		ImGui::SliderFloat("specular2", &m_MeshVector[1].m_Material->m_Specular, 0.f, 1.f); 
		ImGui::SliderFloat("diffuse2", &m_MeshVector[1].m_Material->m_Diffuse, 0.f, 1.f); 
		ImGui::SliderFloat("ambient2", &m_MeshVector[1].m_Material->m_Ambient, 0.f, 1.f);

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
		if(ImGui::Button("enable/disable texture"))
		{
			m_UseTexture = (m_UseTexture == 0) ? 1 : 0;
		}
		if (m_UseTexture == 1)
			ImGui::Text("Using Texture");
		else
			ImGui::Text("Not Using Texture");
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