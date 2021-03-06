#include "Utility.h"

#include <iostream>
#include <algorithm>

namespace Utility
{
	void CreateRotationGenericMatrix(glm::mat4 &mat, float angle, const glm::vec3& axis)
	{
		angle = glm::radians(angle);

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

	void CreateTranslationMatrix(glm::mat4 &mat, glm::vec3 vec)
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

	glm::mat4 CreateOrthoMatrix(float width, float height, float nearVal, float farVal)
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

	glm::mat4 CreatePerspectiveMatrix(float fovy, float aspect, float nearVal, float farVal)
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

	void NormalizeAngle(float& value)
	{
		if (value > 360.f)
		{
			value -= 360.f;
		}
		else if (value < 0.f)
		{
			value += 360.f;
		}
	}

	bool GetPathToRes(std::string& resString)
	{
		bool result = false;
		#ifndef RESPATH
			auto currentPath = std::filesystem::current_path();
			#ifdef DEBUG
				std::cout << "current path: " << currentPath.string() << std::endl;
			#endif
			std::filesystem::path resPath;

			std::filesystem::path engine("OpenGLEngine");
			if (std::search(currentPath.begin(), currentPath.end(), engine.begin(), engine.end()) != currentPath.end()) 
			{
				// engine is a subPath of the currentPath
				for (auto& el : currentPath)
				{
					resPath = resPath / el;
					if (el == "OpenGLEngine")
					{
						resPath = resPath / "engine" / "res";
						resString = resPath.string();
						break;
					}
				}
				result = true;
			}
			// if running the executable out of the project dir, res folder is needed
			else
			{
				resString = "res";
			}
			#ifdef DEBUG
				std::cout << "res path: " << resPath << std::endl;
			#endif
		#else
			// override respath function if a custom path is defined as a preprocessor directive
			resString = RESPATH;
		#endif
		return result;
	}
}
