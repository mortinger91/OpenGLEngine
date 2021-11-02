#pragma once

#ifndef RESPATH
	#include <filesystem>
#endif

#include <string>
#include "vendorLibs.h"

#define REDTEXT(x) std::string("\033[31m")+x+std::string("\033[0m")
#define GREENTEXT(x) std::string("\033[32m")+x+std::string("\033[0m")
#define YELLOWTEXT(x) std::string("\033[33m")+x+std::string("\033[0m")

namespace Utility
{
	void CreateRotationGenericMatrix(glm::mat4 &mat, float angle, const glm::vec3& axis);

	void CreateTranslationMatrix(glm::mat4 &mat, glm::vec3 vec);

	glm::mat4 CreateOrthoMatrix(float width, float height, float nearVal, float farVal);

	glm::mat4 CreatePerspectiveMatrix(float fovy, float aspect, float nearVal, float farVal);

	void NormalizeAngle(float& value);

	bool GetPathToRes(std::string& resString);
}
