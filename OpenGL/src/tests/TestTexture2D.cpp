#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

namespace test
{
	TestTexture2D::TestTexture2D()
		: m_TranslationA(200.f, 200.f, 0.f), m_TranslationB(400.f, 200.f, 0.f),
		// orthographic projection matrix
		m_Proj(glm::ortho(0.f, 960.f, 0.f, 540.f, -1.f, 1.f)),
		m_View(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f)))
	{
		float positions[] = 
		{
			-61.5f, -61.5f, 0.f, 0.f, //0
			 61.5f, -61.5f, 1.f, 0.f, //1
			 61.5f,  61.5f, 1.f, 1.f, //2
			-61.5f,  61.5f, 0.f, 1.f  //3
		};

		unsigned int indices[] = 
		{
			0, 1, 2,
			2, 3, 0
		};

		// initializing vertex array
		m_VAO = std::make_unique<VertexArray>();

		// initializing vertex buffer
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		// initializing vertex buffer layout
		VertexBufferLayout layout;
		// position of the vertices of the two triangles making up a square
		layout.Push<float>(2);
		// coordinates of the texture related to each vertices
		layout.Push<float>(2);
		// adding vertex buffer and layout to vertex array
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		// creating an index buffer
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		// defining a shader
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.vert", "res/shaders/Basic.frag");
		m_Shader->Bind();

		// creating a texture
		m_Texture = std::make_unique<Texture>("res/textures/witcher.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.2f, 0.3f, 0.8f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			// Draw call
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			// Draw call
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		//ImGui::Begin("Hello, world!");
		ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, -960.f, 960.f);
		ImGui::SliderFloat("TranslationA_Y", &m_TranslationA.z, -2.f, 2.f);
		ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, -960.f, 960.f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//ImGui::End();
	}
}