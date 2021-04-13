#pragma once

#include "Test.h"

#include <memory>
#include "Texture.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Mesh.h"

#include "IndexBuffer.h"
#include "VertexArray.h"

namespace test
{
	class Test3DCube: public Test
	{
	public:
		Test3DCube();
		~Test3DCube();

		void OnUpdate(float deltaTime) override;
		void OnRender(GLFWwindow *window, int width, int height);
		void OnImGuiRender() override;

		//void CreateRotationGenericMatrix(glm::mat4 &mat, float angle, const glm::vec3& axis);
		//void CreateScalingMatrix (glm::mat4 &mat, float scale);
		//void CreateTranslationMatrix (glm::mat4 &mat, glm::vec3 vec);
		//void CreateViewMatrix(glm::mat4 &mat);
		//void RotateViewHorizontal(float amount);
		//void RotateViewVertical(float amount);

		glm::mat4 CreateOrthoMatrix(float width, float height, float nearVal, float farVal);
		glm::mat4 CreatePerspectiveMatrix(float fovy, float aspect, float nearVal, float farVal);

	private:
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		Camera m_Camera;
		glm::mat4 m_ProjOrtho, m_ProjPersp;
		float m_NearPlane, m_FarPlane, m_Fov;
		bool m_UseOrtho, m_UseView;
		unsigned int m_IsLight, m_UseTexture;
		std::vector<Mesh> m_MeshVector;
		bool m_MoveLights;

		// materials and lights parameters
			const glm::vec4 one = { 1, 1, 1, 1 };
			const glm::vec4 medium = { 0.5, 0.5, 0.5, 1 };
			const glm::vec4 small = { 0.2f, 0.2f, 0.2f, 1 };
			const float high = 1000.f;
			const glm::vec4 zero = { 0.0, 0.0, 0.0, 1.0 };
			const glm::vec4 light_specular = { 1.f, 1.f, 0.f, 1.f };
			const glm::vec4 light_direction = { 0.f, 0.f, 0.5f, 0.f }; // Dir light 0 in w 
			const glm::vec4 light_specular1 = { 0, 0.5, 1, 1 };
			const glm::vec4 light_position1 = { 400, -0.5, 0, 1 };

			glm::vec4 light0;
			glm::vec4 light1;

			GLuint light0dirn;
			GLuint light0color;
			GLuint light1posn;
			GLuint light1color;
			GLuint ambient;
			GLuint diffuse;
			GLuint specular;
			GLuint shininess;

			float m_Shininess, m_Specular;
			float m_Diffuse, m_Ambient;
	};
}