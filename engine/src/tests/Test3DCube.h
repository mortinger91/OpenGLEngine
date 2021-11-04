#pragma once

#include "Test.h"

#include <memory>
#include <unordered_map>
#include <vector>
#include <thread>
#include "Shader.h"
#include "Material.h"
#include "Camera.h"
#include "Model.h"
#include "vendorLibs.h"

namespace test
{
	class Test3DCube: public Test
	{
	public:
		Test3DCube();
		~Test3DCube();

		// marked as static public member so that ScrollCallback can access it
		// only solution found as now since ScrollCallback can't be a member function and no other arguments can be passed to it
		static std::unique_ptr<Camera> m_Camera;

		void OnUpdate(float deltaTime) override;
		void ProcessInput(GLFWwindow *window, int width, int height) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unordered_map< std::string, std::shared_ptr<Shader> > m_Shaders;
		std::unordered_map< std::string, std::shared_ptr<Material> > m_Materials;
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
		unsigned int m_SelectedModel;

		std::string m_ResPath;
		std::thread* m_LoadingMeshesThread;

		int m_Width;
		int m_Height;
		bool m_ScrollCallbackRegistered;

		bool LoadModels();
	};
}
