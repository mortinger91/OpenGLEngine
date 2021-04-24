#include "Test3DCube.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "cube.h"
#include "Utility.h"
#include <chrono>

namespace test
{
	Test3DCube::Test3DCube()
		: m_NearPlane(1.f),
		  m_FarPlane(1000.f),
		  m_UseOrtho(false),
		  m_UseView(true),
		  m_Fov(30.0f),
		  m_IsLight(1),
		  m_UseTexture(1),
		  m_MoveLights(true),
		  m_Camera(glm::vec3(0.f, 0.f, 60.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)),
		  index(0)
	{
		GLCall(glEnable(GL_BLEND));
		// setting up a blend function, default would be src=0 dest=1 which means override old pixels with new ones
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));
		
		// defining a shader
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.vert", "res/shaders/Basic.frag");
		m_Shader->Bind();

		m_Shader->SetUniform1i("u_Texture", 0);

		Mesh mesh1("Cube", Cube::positions, Cube::colors, Cube::normals, Cube::textures, Cube::indices);
		mesh1.SetMaterial("res/textures/metal.png");
		m_MeshVector.push_back(std::move(mesh1));
		Mesh mesh2("Teapot", "res/meshes/teapot.obj");
		mesh2.SetMaterial("res/textures/metal.png");
		m_MeshVector.push_back(std::move(mesh2));

		m_MeshVector[0].m_Scale = 6.f;
		m_MeshVector[1].m_Scale = 12.f;
	}

	//mat[col][row]

	glm::mat4 Test3DCube::CreateOrthoMatrix(float width, float height, float nearVal, float farVal)
	{
		float left = -width/2;
		float right = width/2; 
		float bottom = -height/2; 
		float top = height/2;
		glm::mat4 mat;
		mat = glm::mat4(0.f);
		mat[0][0] = 2 / (right - left);
		mat[1][1] = 2 / (top - bottom);
		mat[2][2] = -2 / (farVal - nearVal);
		mat[3][0] = -(right + left) / (right - left);
		mat[3][1] = -(top + bottom) / (top - bottom);
		mat[3][2] = -(farVal + nearVal) / (farVal - nearVal);
		mat[3][3] = 1;
		return mat;
	}

	glm::mat4 Test3DCube::CreatePerspectiveMatrix(float fovy, float aspect, float nearVal, float farVal)
	{
		fovy = glm::radians(fovy);
		glm::mat4 mat;
		mat = glm::mat4(0.f);
		float tanFovy = glm::tan(fovy/2);
		mat[0][0] = 1 / (tanFovy * aspect);
		mat[1][1] = 1 / tanFovy;
		mat[2][2] = - (farVal + nearVal)/(farVal - nearVal);
		mat[3][2] = - (2 * farVal * nearVal)/(farVal - nearVal);
		mat[2][3] = -1;
		return mat;
	}

	void Test3DCube::OnRender(GLFWwindow *window, int width, int height)
	{
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_Camera.RotateViewHorizontal(1.f);
		}
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_Camera.RotateViewHorizontal(-1.f);
		}
			
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_Camera.RotateViewVertical(1.f);
		}
		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_Camera.RotateViewVertical(-1.f);
		}

		GLCall(glClearColor(0.2f, 0.3f, 0.8f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));

		{
			m_Camera.CreateViewMatrix();

			for(int i = 0; i < m_MeshVector.size(); ++i)
			{
				//m_MeshVector[i].Bind();

				m_MeshVector[i].CreateModelMatrix();

				glm::mat4 mv;
				glm::mat4 mvp;
			
				if (m_UseView)
				{
					mv = m_Camera.m_ViewMatrix * m_MeshVector[i].m_ModelMatrix;
				}
				else
				{
					mv = m_MeshVector[i].m_ModelMatrix;
				}

				// Set Light and Material properties for the teapot
				// Lights are transformed by current modelview matrix. 
				// The shader can't do this globally. 
				// So we need to do so manually.
				if (m_MoveLights)
				{
					light0 = mv * light_direction;
					light1 = mv * light_position1;
				}
				else
				{
					light0 = light_direction;
					light1 = light_position1;
				}

				if(m_UseOrtho)
				{
					m_ProjOrtho = CreateOrthoMatrix((float)width, (float)height, m_NearPlane, m_FarPlane);
					mvp = m_ProjOrtho * mv;
				}
				else
				{
					m_ProjPersp = CreatePerspectiveMatrix(m_Fov, (float)width / (float)height, m_NearPlane, m_FarPlane);
					mvp = m_ProjPersp * mv;
				}

				m_Shader->SetUniformMat4f("u_MV", mv);
				m_Shader->SetUniformMat4f("u_MVP", mvp);
				m_Shader->SetUniform1i("useTexture", m_UseTexture);
				m_Shader->SetUniform1i("islight", m_IsLight);
				m_Shader->SetUniform3fv("light0dirn", light0);
				m_Shader->SetUniform4fv("light0color", light_specular);
				m_Shader->SetUniform4fv("light1posn", light1);
				m_Shader->SetUniform4fv("light1color", light_specular1);
				m_Shader->SetUniform4fv("ambient", { m_MeshVector[i].m_Material->m_Ambient, m_MeshVector[i].m_Material->m_Ambient, m_MeshVector[i].m_Material->m_Ambient, 1 });
				m_Shader->SetUniform4fv("diffuse", { m_MeshVector[i].m_Material->m_Diffuse, m_MeshVector[i].m_Material->m_Diffuse, m_MeshVector[i].m_Material->m_Diffuse, 1 });
				m_Shader->SetUniform4fv("specular", { m_MeshVector[i].m_Material->m_Specular, m_MeshVector[i].m_Material->m_Specular, m_MeshVector[i].m_Material->m_Specular, 1 });
				m_Shader->SetUniform1fv("shininess", m_MeshVector[i].m_Material->m_Shininess);

				renderer.Draw(m_MeshVector[i], *m_Shader);
			}
		}
	}

	void Test3DCube::OnImGuiRender()
	{
		ImGui::SliderFloat("Translate X1", &m_MeshVector[0].m_TranslationVec.x, -100.f, 100.f);
		ImGui::SliderFloat("Translate Y1", &m_MeshVector[0].m_TranslationVec.y, -100.f, 100.f);
		ImGui::SliderFloat("Translate Z1", &m_MeshVector[0].m_TranslationVec.z, -1000.f, 1000.f);
		ImGui::SliderFloat3("Model Rotation1", &m_MeshVector[0].m_RotationVec.x, -360.f, 360.f);
		ImGui::SliderFloat("Scale1", &m_MeshVector[0].m_Scale, 0.f, 50.f);

		ImGui::SliderFloat("Translate X2", &m_MeshVector[1].m_TranslationVec.x, -100.f, 100.f);
		ImGui::SliderFloat("Translate Y2", &m_MeshVector[1].m_TranslationVec.y, -100.f, 100.f);
		ImGui::SliderFloat("Translate Z2", &m_MeshVector[1].m_TranslationVec.z, -1000.f, 1000.f);
		ImGui::SliderFloat3("Model Rotation2", &m_MeshVector[1].m_RotationVec.x, -360.f, 360.f);
		ImGui::SliderFloat("Scale2", &m_MeshVector[1].m_Scale, 0.f, 50.f);

		if(ImGui::Button("rotate view horizontal"))
		{
			m_Camera.RotateViewHorizontal(5.f);
		}
		if(ImGui::Button("rotate view vertical"))
		{
			m_Camera.RotateViewVertical(5.f);
		}

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
		if(ImGui::Button("change proj matrix"))
		{
			m_UseOrtho = !m_UseOrtho;
		}
		if (m_UseOrtho)
			ImGui::Text("Using Ortho");
		else
			ImGui::Text("Using Perspective");
		if(ImGui::Button("enable/disable view"))
		{
			m_UseView = !m_UseView;
		}
		if (m_UseView)
			ImGui::Text("Using View");
		else
			ImGui::Text("Not Using View");
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
		if(ImGui::Button("Move Lights"))
		{
			m_MoveLights = !m_MoveLights;
		}
		if (m_MoveLights)
			ImGui::Text("Moving lights");
		else
			ImGui::Text("Not moving lights");
		//if(ImGui::Button("change selected mesh"))
		//{
		//	index = (index == 0) ? 1 : 0;
		//}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	Test3DCube::~Test3DCube()
	{
	}

	void Test3DCube::OnUpdate(float deltaTime)
	{
	}

}