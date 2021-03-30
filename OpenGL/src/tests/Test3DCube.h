#pragma once

#include "Test.h"

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Camera.h"

#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	class Test3DCube: public Test
	{
	public:
		Test3DCube();
		~Test3DCube();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void CreateRotationZMatrix (glm::mat4 &mat, float angle);
		void CreateRotationYMatrix (glm::mat4 &mat, float angle);
		void CreateRotationXMatrix (glm::mat4 &mat, float angle);
		void CreateRotationGenericMatrix(glm::mat4 &mat, float angle, const glm::vec3& axis);
		void CreateScalingMatrix (glm::mat4 &mat, float scale);
		void CreateTranslationMatrix (glm::mat4 &mat, glm::vec3 vec);

		void CreateModelMatrix(glm::mat4 &mat, glm::vec3 rotationVec, glm::vec3 translationVec, float scale);

		void CreateViewMatrix(glm::mat4 &mat, glm::vec3 eyeVec, glm::vec3 centerVec, glm::vec3 upVec);

		glm::mat4 CreateOrthoMatrix(float left, float right, float bottom, float top, float nearVal, float farVal);

		glm::mat4 CreatePerspectiveMatrix(float fovy, float aspect, float nearVal, float farVal);

	private:
		glm::vec3 m_TranslationVec, m_RotationVec;
		glm::vec3 eyeVec, centerVec, upVec;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<Camera> m_Camera;
		glm::mat4 m_ProjOrtho, m_ProjPersp;
		float m_Scale, m_NearPlane, m_FarPlane, m_Fov;
		bool m_UseOrtho, m_UseView;
		//float r, increment;
	};
}