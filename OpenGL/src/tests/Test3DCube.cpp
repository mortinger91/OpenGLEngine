#include "Test3DCube.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

//void glhLookAtf2( float *matrix, float *eyePosition3D,
//                  float *center3D, float *upVector3D )
//{
//   float forward[3], side[3], up[3];
//   float matrix2[16], resultMatrix[16];
//   // --------------------
//   forward[0] = center3D[0] - eyePosition3D[0];
//   forward[1] = center3D[1] - eyePosition3D[1];
//   forward[2] = center3D[2] - eyePosition3D[2];
//   NormalizeVector(forward);
//   // --------------------
//   // Side = forward x up
//   ComputeNormalOfPlane(side, forward, upVector3D);
//   NormalizeVector(side);
//   --------------------
//   // Recompute up as: up = side x forward
//   ComputeNormalOfPlane(up, side, forward);
//   // --------------------
//   matrix2[0] = side[0];
//   matrix2[4] = side[1];
//   matrix2[8] = side[2];
//   matrix2[12] = 0.0;
//   // --------------------
//   matrix2[1] = up[0];
//   matrix2[5] = up[1];
//   matrix2[9] = up[2];
//   matrix2[13] = 0.0;
//   // --------------------
//   matrix2[2] = -forward[0];
//   matrix2[6] = -forward[1];
//   matrix2[10] = -forward[2];
//   matrix2[14] = 0.0;
//   // --------------------
//   matrix2[3] = matrix2[7] = matrix2[11] = 0.0;
//   matrix2[15] = 1.0;
//   // --------------------
//   MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);
//   glhTranslatef2(resultMatrix,
//                  -eyePosition3D[0], -eyePosition3D[1], -eyePosition3D[2]);
//   // --------------------
//   memcpy(matrix, resultMatrix, 16*sizeof(float));
//}

void NormalizeVector(glm::vec3 &vec)
{
	vec = glm::normalize(vec);

	//float sqrt = glm::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	//if (sqrt == 0) return;
	//vec.x /= sqrt;
	//vec.y /= sqrt;
	//vec.z /= sqrt;
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
		: m_TranslationVec(-200.f, -60.f, -700.f),
		  m_RotationVec(0.f, 0.f, 0.f),
		  m_Scale(100.f),
		  //m_View(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f))),
		  //eyeVec(10.f, 10.f, 100.f),
		  eyeVec(0.f, 0.f, 0.f),
		  //centerVec(0.f, 0.f, 0.f),
		  centerVec(-10.f, -10.f, -100.f),
		  upVec(0.f, 1.f, 0.f),
		  //m_ProjOrtho(glm::ortho(-480., 480., -270., 270., -1000., 1000.)),
		  //m_ProjPersp(glm::perspective(90.0f * glm::pi<float>() / 180.0f, (float)960 / (float)540, -1000.f, 1000.f)),
		  m_NearPlane(1.f),
		  m_FarPlane(5000.f),
		  m_UseOrtho(false),
		  m_UseView(true),
		  m_Fov(30.0f)

	{
		float positions[] =
		{
			  // front
			  1.0f,   1.0f,   1.0f,   1.f,   0.f,   0.f,  0.0f,  0.0f,  1.0f,  // 0.f,  0.f, //0 //0
			 -1.0f,   1.0f,   1.0f,   1.f,   0.f,   0.f,  0.0f,  0.0f,  1.0f,  // 0.f,  0.f, //1
			 -1.0f,  -1.0f,   1.0f,   1.f,   0.f,   0.f,  0.0f,  0.0f,  1.0f,  // 0.f,  0.f, //2
			  1.0f,  -1.0f,   1.0f,   1.f,   0.f,   0.f,  0.0f,  0.0f,  1.0f,  // 0.f,  0.f, //3

			  // left				 			   
			  1.0f,   1.0f,   1.0f,   0.f,   1.f,   0.f, -1.0f,  0.0f,  0.0f,  // 1.f,  0.f, //0 //4
			  1.0f,  -1.0f,   1.0f,   0.f,   1.f,   0.f, -1.0f,  0.0f,  0.0f,  // 1.f,  0.f, //3
			  1.0f,  -1.0f,  -1.0f,   0.f,   1.f,   0.f, -1.0f,  0.0f,  0.0f,  // 1.f,  0.f, //7
			  1.0f,   1.0f,  -1.0f,   0.f,   1.f,   0.f, -1.0f,  0.0f,  0.0f,  // 1.f,  0.f, //4

			  // right					     	   
			 -1.0f,   1.0f,   1.0f,   0.f,   0.f,   1.f,  1.0f,  0.0f,  0.0f,  // 1.f,  0.f, //1 //8
			 -1.0f,  -1.0f,   1.0f,   0.f,   0.f,   1.f,  1.0f,  0.0f,  0.0f,  // 1.f,  0.f, //2
			 -1.0f,  -1.0f,  -1.0f,   0.f,   0.f,   1.f,  1.0f,  0.0f,  0.0f,  // 1.f,  0.f, //6
			 -1.0f,   1.0f,  -1.0f,   0.f,   0.f,   1.f,  1.0f,  0.0f,  0.0f,  // 1.f,  0.f, //5

			  // bot					     	   
			  1.0f,  -1.0f,   1.0f,   1.f,   1.f,   0.f,  0.0f, -1.0f,  0.0f,  //  0.f,  1.f, //3 //12
			 -1.0f,  -1.0f,   1.0f,   1.f,   1.f,   0.f,  0.0f, -1.0f,  0.0f,  //  0.f,  1.f, //2
			  1.0f,  -1.0f,  -1.0f,   1.f,   1.f,   0.f,  0.0f, -1.0f,  0.0f,  //  0.f,  1.f, //7
			 -1.0f,  -1.0f,  -1.0f,   1.f,   1.f,   0.f,  0.0f, -1.0f,  0.0f,  //  0.f,  1.f, //6

			  // top					     	   
			  1.0f,   1.0f,   1.0f,   1.f,   0.f,   1.f,  0.0f,  1.0f,  0.0f,  //  0.f,  1.f, //0 //16
			 -1.0f,   1.0f,   1.0f,   1.f,   0.f,   1.f,  0.0f,  1.0f,  0.0f,  //  0.f,  1.f, //1
			  1.0f,   1.0f,  -1.0f,   1.f,   0.f,   1.f,  0.0f,  1.0f,  0.0f,  //  0.f,  1.f, //4
			 -1.0f,   1.0f,  -1.0f,   1.f,   0.f,   1.f,  0.0f,  1.0f,  0.0f,  //  0.f,  1.f, //5

			  // back					     	   
			  1.0f,   1.0f,  -1.0f,   0.f,   1.f,   1.f,  0.0f,  0.0f, -1.0f,  //  0.f,  0.f, //4 //20
			 -1.0f,   1.0f,  -1.0f,   0.f,   1.f,   1.f,  0.0f,  0.0f, -1.0f,  //  0.f,  0.f, //5
			 -1.0f,  -1.0f,  -1.0f,   0.f,   1.f,   1.f,  0.0f,  0.0f, -1.0f,  //  0.f,  0.f, //6
			  1.0f,  -1.0f,  -1.0f,   0.f,   1.f,   1.f,  0.0f,  0.0f, -1.0f   //  0.f,  0.f  //7
		};

		unsigned int indices[] =
		{
			0, 1, 3,  2, 1, 3, // front
			4, 7, 5,  6, 7, 5, // left
			8, 9, 11, 10, 9, 11, // right
			12, 13, 14, 15, 13, 14, // bot
			16, 17, 18, 19, 17, 18, //top
			20, 21, 23, 22, 21, 23 // back
		};

		GLCall(glEnable(GL_BLEND));
		// setting up a blend function, default would be src=0 dest=1 which means override old pixels with new ones
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));

		// initializing vertex array
		m_VAO = std::make_unique<VertexArray>();

		// initializing vertex buffer
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 24 * 9 * sizeof(float));
		// initializing vertex buffer layout
		VertexBufferLayout layout;
		// position of the vertices of the two triangles making up a square
		// layout(location = 0) in vertex shader
		layout.Push<float>(3);
		// coordinates of the texture related to each vertices
		// layout(location = 1) in vertex shader
		layout.Push<float>(3);
		layout.Push<float>(3);
		// adding vertex buffer and layout to vertex array
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		// creating an index buffer
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 30);

		// defining a shader
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.vert", "res/shaders/Basic.frag");
		m_Shader->Bind();

		// creating a texture
		//m_Texture = std::make_unique<Texture>("res/textures/witcher.png");
		//m_Shader->SetUniform1i("u_Texture", 0);

		//m_Shader->SetUniform1i("u_UseTexture", 0);
		//r = 1.0f;
		//increment = 0.05f;

	}

	//mat[col][row]

	void Test3DCube::CreateRotationZMatrix(glm::mat4 &mat, float angle)
	{
		mat = glm::mat4(0.f);
		mat[0][0] = glm::cos(angle);
		mat[1][0] = -glm::sin(angle);
		mat[0][1] = glm::sin(angle);
		mat[1][1] = glm::cos(angle);
		mat[2][2] = 1;
		mat[3][3] = 1;
	}

	void Test3DCube::CreateRotationYMatrix(glm::mat4 &mat, float angle)
	{
		mat = glm::mat4(0.f);
		mat[0][0] = glm::cos(angle);
		mat[2][0] = glm::sin(angle);
		mat[1][1] = 1;
		mat[0][2] = -glm::sin(angle);
		mat[2][2] = glm::cos(angle);
		mat[3][3] = 1;
	}

	void Test3DCube::CreateRotationXMatrix(glm::mat4 &mat, float angle)
	{
		mat = glm::mat4(0.f);
		mat[0][0] = 1;
		mat[1][1] = glm::cos(angle);
		mat[2][1] = -glm::sin(angle);
		mat[1][2] = glm::sin(angle);
		mat[2][2] = glm::cos(angle);
		mat[3][3] = 1;
	}

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

		//CreateRotationZMatrix(matRotateZ, rotationVec.z);
		CreateRotationGenericMatrix(matRotateZ, rotationVec.z, glm::vec3(0,0,1));
		//CreateRotationYMatrix(matRotateY, rotationVec.y);
		CreateRotationGenericMatrix(matRotateY, rotationVec.y, glm::vec3(0,1,0));
		//CreateRotationXMatrix(matRotateX, rotationVec.x);
		CreateRotationGenericMatrix(matRotateX, rotationVec.x, glm::vec3(1,0,0));

		CreateScalingMatrix(matScale, scale);

		mat = matTranslate * matRotateY * matRotateZ * matRotateX * matScale;
	}

	void Test3DCube::CreateViewMatrix(glm::mat4 &mat, glm::vec3 eyeVec, glm::vec3 centerVec, glm::vec3 upVec)
	{
		glm::vec3 w = eyeVec - centerVec;
		//NormalizeVector(w);
		w = glm::normalize(w);
		glm::vec3 u = upVec;
		u = glm::cross(u, w);
		//NormalizeVector(u);
		u = glm::normalize(u);
		glm::vec3 v = glm::cross(w, u);

		mat[0][0] = u.x;
		mat[1][0] = u.y;
		mat[2][0] = u.z;
		mat[3][0] = -glm::dot(u, eyeVec); //-u.x*eyeVec.x - u.y*eyeVec.y - u.z*eyeVec.z;
		mat[0][1] = v.x;
		mat[1][1] = v.y;
		mat[2][1] = v.z;
		mat[3][1] = -glm::dot(v, eyeVec); //-v.x*eyeVec.x - v.y*eyeVec.y - v.z*eyeVec.z;
		mat[0][2] = w.x;
		mat[1][2] = w.y;
		mat[2][2] = w.z;
		mat[3][2] = -glm::dot(w, eyeVec); //-w.x*eyeVec.x - w.y*eyeVec.y - w.z*eyeVec.z;
		mat[0][3] = 0;
		mat[1][3] = 0;
		mat[2][3] = 0;
		mat[3][3] = 1;
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

	void Test3DCube::OnRender()
	{
		GLCall(glClearColor(0.2f, 0.3f, 0.8f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));

		Renderer renderer;

		//m_Texture->Bind();

		{
			glm::mat4 model;
			CreateModelMatrix(model, m_RotationVec, m_TranslationVec, m_Scale);

			glm::mat4 view;
			CreateViewMatrix(view, eyeVec, centerVec, upVec);

			glm::mat4 mvp = model;

			if (m_UseView)
				mvp = view * mvp;

			// Set Light and Material properties for the teapot
			// Lights are transformed by current modelview matrix. 
			// The shader can't do this globally. 
			// So we need to do so manually.  
			transformvec(light_direction, light0, mvp);
			transformvec(light_position1, light1, mvp);

			m_Shader->SetUniform3fv("light0dirn", light0);
			m_Shader->SetUniform4fv("light0color", light_specular);
			m_Shader->SetUniform4fv("light1posn", light1);
			m_Shader->SetUniform4fv("light1color", light_specular1);

			m_Shader->SetUniform4fv("ambient", small);
			m_Shader->SetUniform4fv("diffuse", medium);
			m_Shader->SetUniform4fv("specular", one);
			m_Shader->SetUniform1fv("shininess", high);

			if(m_UseOrtho)
			{
				m_ProjOrtho = CreateOrthoMatrix(-480., 480., -270., 270., m_NearPlane, m_FarPlane);
				mvp = m_ProjOrtho * mvp; // * position
			}
			else
			{
				//m_ProjPersp = glm::perspective(90.0f * glm::pi<float>() / 180.0f, (float)960 / (float)540, m_NearPlane, m_FarPlane);
				m_ProjPersp = CreatePerspectiveMatrix(m_Fov * glm::pi<float>() / 180.0f, (float)960 / (float)540, m_NearPlane, m_FarPlane);
				mvp = m_ProjPersp * mvp; // * position
			}

			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			//m_Shader->SetUniform1f("u_ChangingColor", r);
			//if (r > 1.f)
			//	increment = -0.05f;
			//else if (r < 0.f)
			//	increment = 0.05f;

			//r += increment;

			//glm::vec3 eye(0.f, 0.f, 0.f), center(0.f, 0.f, -1.f), up(0.f, 1.f, 0.f);
			//0, 0, 0, 0, 0,-1, 0, 1, 0
			//GLCall(gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z));

			// Draw call
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void Test3DCube::OnImGuiRender()
	{
		ImGui::SliderFloat("Translate X", &m_TranslationVec.x, -1000.f, 1000.f);
		ImGui::SliderFloat("Translate Y", &m_TranslationVec.y, -1000.f, 1000.f);
		ImGui::SliderFloat("Translate Z", &m_TranslationVec.z, -1000.f, 1000.f);
		ImGui::SliderFloat("Rotate horizontal", &m_RotationVec.y, 0.f, 360.f);
		ImGui::SliderFloat("angleZ", &m_RotationVec.z, 0.f, 360.f);
		ImGui::SliderFloat("angleX", &m_RotationVec.x, 0.f, 360.f);
		ImGui::SliderFloat("scale", &m_Scale, 0.f, 500.f);
		ImGui::SliderFloat("nearPlane", &m_NearPlane, -500.f, 500.f);
		ImGui::SliderFloat("farPlane", &m_FarPlane, 0.f, 5000.f);
		ImGui::SliderFloat("fovy", &m_Fov, 0.f, 360.f);
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
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	Test3DCube::~Test3DCube()
	{
	}

	void Test3DCube::OnUpdate(float deltaTime)
	{
	}

}