#pragma once

#include "Test.h"

#include <memory>
#include <unordered_map>
#include <vector>
#include "Shader.h"
#include "Material.h"
#include "Camera.h"
#include "Model.h"
//#include "Texture.h"
//#include "IndexBuffer.h"
//#include "VertexArray.h"

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

	private:
		std::unordered_map< std::string, std::shared_ptr<Shader> > m_Shaders;
		std::unordered_map< std::string, std::shared_ptr<Material> > m_Materials;
		std::unique_ptr<Camera> m_Camera;
		std::vector<Model> m_Models;
		Renderer m_Renderer;
		
		glm::mat4 m_ProjMatrix;
		float m_NearPlane, m_FarPlane, m_Fov;

		bool m_UseOrtho, m_StopRotation, m_RotatingPointLight;
		unsigned int m_IsLight;

		glm::vec4 light_direction = { 0.f, 0.f, 0.5f, 0.f }; // Dir light 0 in w 
		glm::vec4 light_color = { 0.f, 0.f, 0.f, 1.f };

		glm::vec4 light_position1 = { 30, 0, 0, 1 };
		glm::vec4 light_color1 = { 1.5f, 1.5f, 1.5f, 1.f };

		float m_PointLightAngle;
	};
}