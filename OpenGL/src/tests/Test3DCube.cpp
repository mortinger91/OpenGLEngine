#include "Test3DCube.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "cube.h"

void NormalizeVector(glm::vec3 &vec)
{
	//float sqrt = glm::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	//if (sqrt == 0) return;
	//vec.x /= sqrt;
	//vec.y /= sqrt;
	//vec.z /= sqrt;
	vec = glm::normalize(vec);
}

void ConvertToRadians(float &value)
{
	//float degrees = (angle * glm::pi<float>()) / 180.f;
	value = glm::radians(value);
}

/* New helper transformation function to transform vector by modelview */ 
void transformvec (const glm::vec4 input, glm::vec4 output, const glm::mat4& modelview) 
{
	glm::vec4 inputvec(input[0], input[1], input[2], input[3]);
	glm::vec4 outputvec = modelview * inputvec;
	output[0] = outputvec[0];
	output[1] = outputvec[1];
	output[2] = outputvec[2];
	output[3] = outputvec[3];
}

namespace test
{
	Test3DCube::Test3DCube()
		//: m_TranslationVec(0.f, 0.f, -520.f),
		: m_TranslationVec(0.f, 0.f, 0.f),
		  m_RotationVec(0.f, 0.f, 0.f),
		  m_Scale(6.f),
		  m_EyeVec(0.f, 0.f, 60.f),
		  //m_CenterVec(-10.f, -10.f, -100.f),
		  m_CenterVec(0.f, 0.f, 0.f),
		  m_UpVec(0.f, 1.f, 0.f),
		  m_NearPlane(1.f),
		  m_FarPlane(1000.f),
		  m_UseOrtho(false),
		  m_UseView(true),
		  m_Fov(30.0f),
		  m_IsLight(1),
		  m_Shininess(100.f),
		  m_Specular(0.75f),
		  m_Diffuse(0.5f),
		  m_Ambient(0.2f),
		  m_UseTexture(1),
		  // creating a mesh
		  m_Mesh1(Cube::positions, Cube::colors, Cube::normals, Cube::textures, Cube::indices),
		  m_Mesh2("res/meshes/teapot.obj"),
		  m_ChooseMesh(true)

	{
		
		GLCall(glEnable(GL_BLEND));
		// setting up a blend function, default would be src=0 dest=1 which means override old pixels with new ones
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));

		// defining a shader
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.vert", "res/shaders/Basic.frag");
		m_Shader->Bind();

		// creating a texture
		m_Texture = std::make_unique<Texture>("res/textures/metal.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	//mat[col][row]

	//void Test3DCube::CreateRotationZMatrix(glm::mat4 &mat, float angle)
	//{
	//	mat = glm::mat4(0.f);
	//	mat[0][0] = glm::cos(angle);
	//	mat[1][0] = -glm::sin(angle);
	//	mat[0][1] = glm::sin(angle);
	//	mat[1][1] = glm::cos(angle);
	//	mat[2][2] = 1;
	//	mat[3][3] = 1;
	//}

	//void Test3DCube::CreateRotationYMatrix(glm::mat4 &mat, float angle)
	//{
	//	mat = glm::mat4(0.f);
	//	mat[0][0] = glm::cos(angle);
	//	mat[2][0] = glm::sin(angle);
	//	mat[1][1] = 1;
	//	mat[0][2] = -glm::sin(angle);
	//	mat[2][2] = glm::cos(angle);
	//	mat[3][3] = 1;
	//}

	//void Test3DCube::CreateRotationXMatrix(glm::mat4 &mat, float angle)
	//{
	//	mat = glm::mat4(0.f);
	//	mat[0][0] = 1;
	//	mat[1][1] = glm::cos(angle);
	//	mat[2][1] = -glm::sin(angle);
	//	mat[1][2] = glm::sin(angle);
	//	mat[2][2] = glm::cos(angle);
	//	mat[3][3] = 1;
	//}

	void Test3DCube::CreateRotationGenericMatrix(glm::mat4 &mat, float angle, const glm::vec3& axis)
	{
		ConvertToRadians(angle);

		float cos = glm::cos(angle);
		float sin = glm::sin(angle);

		mat = glm::mat4(0.f);
		mat[0][0] = cos + (1-cos)*(axis.x*axis.x);
		mat[1][0] = (1-cos)*(axis.x*axis.y) - sin*axis.z;
		mat[2][0] = (1-cos)*(axis.x*axis.z) + sin*axis.y;
		mat[0][1] = (1-cos)*(axis.x*axis.y) + sin*axis.z;
		mat[1][1] = cos + (1-cos)*(axis.y*axis.y);
		mat[2][1] = (1-cos)*(axis.y*axis.z) - sin*axis.x;
		mat[0][2] = (1-cos)*(axis.x*axis.z) - sin*axis.y;
		mat[1][2] = (1-cos)*(axis.y*axis.z) + sin*axis.x;
		mat[2][2] = cos + (1-cos)*(axis.z*axis.z);
		mat[3][3] = 1;
	}

	void Test3DCube::CreateScalingMatrix(glm::mat4 &mat, float scale)
	{
		mat = glm::mat4(0.f);
		mat[0][0] = scale;
		mat[1][1] = scale;
		mat[2][2] = scale;
		mat[3][3] = 1;
	}

	void Test3DCube::CreateTranslationMatrix(glm::mat4 &mat, glm::vec3 vec)
	{
		mat = glm::mat4(0.f);
		mat[0][0] = 1;
		mat[3][0] = vec.x;
		mat[1][1] = 1;
		mat[3][1] = vec.y;
		mat[2][2] = 1;
		mat[3][2] = vec.z;
		mat[3][3] = 1;
	}

	void Test3DCube::CreateModelMatrix(glm::mat4 &mat, glm::vec3 rotationVec, glm::vec3 translationVec, float scale)
	{
		glm::mat4 matTranslate;
		glm::mat4 matRotateZ;
		glm::mat4 matRotateY;
		glm::mat4 matRotateX;
		glm::mat4 matScale;

		CreateTranslationMatrix(matTranslate, translationVec);

		// TODO: use quaternions to perform model rotation in order to avoid gimbal lock
		CreateRotationGenericMatrix(matRotateZ, rotationVec.z, glm::vec3(0,0,1));
		CreateRotationGenericMatrix(matRotateY, rotationVec.y, glm::vec3(0,1,0));
		CreateRotationGenericMatrix(matRotateX, rotationVec.x, glm::vec3(1,0,0));

		CreateScalingMatrix(matScale, scale);

		mat = matTranslate * matRotateY * matRotateZ * matRotateX * matScale;
	}

	void Test3DCube::CreateViewMatrix(glm::mat4 &mat)//, glm::vec3 m_EyeVec, glm::vec3 m_CenterVec, glm::vec3 m_UpVec)
	{
		glm::vec3 w = glm::normalize(m_EyeVec - m_CenterVec);
		glm::vec3 u = glm::normalize(glm::cross(m_UpVec, w));
		glm::vec3 v = glm::cross(w, u);

		mat[0][0] = u.x;
		mat[1][0] = u.y;
		mat[2][0] = u.z;
		mat[3][0] = -glm::dot(u, m_EyeVec); //-u.x*m_EyeVec.x - u.y*m_EyeVec.y - u.z*m_EyeVec.z;
		mat[0][1] = v.x;
		mat[1][1] = v.y;
		mat[2][1] = v.z;
		mat[3][1] = -glm::dot(v, m_EyeVec); //-v.x*m_EyeVec.x - v.y*m_EyeVec.y - v.z*m_EyeVec.z;
		mat[0][2] = w.x;
		mat[1][2] = w.y;
		mat[2][2] = w.z;
		mat[3][2] = -glm::dot(w, m_EyeVec); //-w.x*m_EyeVec.x - w.y*m_EyeVec.y - w.z*m_EyeVec.z;
		mat[0][3] = 0;
		mat[1][3] = 0;
		mat[2][3] = 0;
		mat[3][3] = 1;
	}

	void Test3DCube::RotateViewHorizontal(float amount)
	{
		glm::mat4 matRotation;
		CreateRotationGenericMatrix(matRotation, amount, m_UpVec);
		m_EyeVec = glm::mat3(matRotation) * m_EyeVec;
	}

	void Test3DCube::RotateViewVertical(float amount)
	{
		glm::mat4 matRotation;
		CreateRotationGenericMatrix(matRotation, amount, glm::normalize(glm::cross(m_UpVec, glm::normalize(m_EyeVec)))
		);
		m_EyeVec = glm::mat3(matRotation) * m_EyeVec;
		m_UpVec = glm::mat3(matRotation) * m_UpVec;
	}

	glm::mat4 Test3DCube::CreateOrthoMatrix(float left, float right, float bottom, float top, float nearVal, float farVal)
	{
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
			RotateViewHorizontal(1.f);
		}
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			RotateViewHorizontal(-1.f);
		}
			
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			RotateViewVertical(1.f);
		}
		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			RotateViewVertical(-1.f);
		}

		GLCall(glClearColor(0.2f, 0.3f, 0.8f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();

		{
			m_Shader->Bind();

			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 mv;
			glm::mat4 mvp;

			CreateModelMatrix(model, m_RotationVec, m_TranslationVec, m_Scale);

			CreateViewMatrix(view);
			
			if (m_UseView)
				mv = view * model;
			else
				mv = model;

			m_Shader->SetUniformMat4f("u_MV", mv);

			// Set Light and Material properties for the teapot
			// Lights are transformed by current modelview matrix. 
			// The shader can't do this globally. 
			// So we need to do so manually.  
			//transformvec(light_direction, light0, mv);
			light0 = light_direction;
			//transformvec(light_position1, light1, mvp);
			light1 = light_position1;

			if(m_UseOrtho)
			{
				m_ProjOrtho = CreateOrthoMatrix(-480., 480., -270., 270., m_NearPlane, m_FarPlane);
				mvp = m_ProjOrtho * mv;
			}
			else
			{
				m_ProjPersp = CreatePerspectiveMatrix(m_Fov * glm::pi<float>() / 180.0f, (float)960 / (float)540, m_NearPlane, m_FarPlane);
				mvp = m_ProjPersp * mv;
			}

			m_Shader->SetUniformMat4f("u_MVP", mvp);

			m_Shader->SetUniform1i("islight", m_IsLight);
			m_Shader->SetUniform3fv("light0dirn", light0);
			m_Shader->SetUniform4fv("light0color", light_specular);
			m_Shader->SetUniform4fv("light1posn", light1);
			m_Shader->SetUniform4fv("light1color", light_specular1);

			m_Shader->SetUniform4fv("ambient", { m_Ambient, m_Ambient, m_Ambient, 1 });
			m_Shader->SetUniform4fv("diffuse", { m_Diffuse, m_Diffuse, m_Diffuse, 1 });
			m_Shader->SetUniform4fv("specular", { m_Specular, m_Specular, m_Specular, 1 });
			m_Shader->SetUniform1fv("shininess", m_Shininess);

			m_Shader->SetUniform1i("useTexture", m_UseTexture);

			// Draw call
			if (m_ChooseMesh)
				renderer.Draw(*(m_Mesh1.m_VAO), *(m_Mesh1.m_IndexBuffer), *m_Shader);
			else
				renderer.Draw(*(m_Mesh2.m_VAO), *(m_Mesh2.m_IndexBuffer), *m_Shader);
		}
	}

	void Test3DCube::OnImGuiRender()
	{
		ImGui::SliderFloat("Translate X", &m_TranslationVec.x, -100.f, 100.f);
		ImGui::SliderFloat("Translate Y", &m_TranslationVec.y, -100.f, 100.f);
		ImGui::SliderFloat("Translate Z", &m_TranslationVec.z, -1000.f, 1000.f);
		ImGui::SliderFloat3("Model Rotation", &m_RotationVec.x, -360.f, 360.f);
		ImGui::SliderFloat("scale", &m_Scale, 0.f, 50.f);
		ImGui::SliderFloat3("m_CenterVec", &m_CenterVec.x, -100.f, 100.f);
		ImGui::SliderFloat3("m_EyeVec", &m_EyeVec.x, -100.f, 100.f);
		ImGui::SliderFloat3("m_UpVec", &m_UpVec.x, -100.f, 100.f);

		if(ImGui::Button("rotate view horizontal"))
		{
			RotateViewHorizontal(5.f);
		}
		if(ImGui::Button("rotate view vertical"))
		{
			RotateViewVertical(5.f);
		}

		ImGui::SliderFloat("nearPlane", &m_NearPlane, -50.f, 50.f);
		ImGui::SliderFloat("farPlane", &m_FarPlane, 0.f, 1000.f);
		ImGui::SliderFloat("fovy", &m_Fov, 0.f, 90.f);
		ImGui::SliderFloat("shininess", &m_Shininess, 0.f, 1000.f); 
		ImGui::SliderFloat("specular", &m_Specular, 0.f, 1.f); 
		ImGui::SliderFloat("diffuse", &m_Diffuse, 0.f, 1.f); 
		ImGui::SliderFloat("ambient", &m_Ambient, 0.f, 1.f); 
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

		if(ImGui::Button("m_ChooseMesh"))
		{
			m_ChooseMesh = !m_ChooseMesh;
		}
		if (m_ChooseMesh)
			ImGui::Text("Using Mesh1");
		else
			ImGui::Text("Using Mesh2");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	Test3DCube::~Test3DCube()
	{
	}

	void Test3DCube::OnUpdate(float deltaTime)
	{
	}

}