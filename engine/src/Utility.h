#pragma once

#ifndef RESPATH
	#include <filesystem>
#endif

#include <string>
#include "vendorLibs.h"

// TODO: Add colors to windows console
#ifdef _WIN32
	#define REDTEXT(x) x
	#define GREENTEXT(x) x
	#define YELLOWTEXT(x) x
#else
	#define REDTEXT(x) std::string("\033[31m")+x+std::string("\033[0m")
	#define GREENTEXT(x) std::string("\033[32m")+x+std::string("\033[0m")
	#define YELLOWTEXT(x) std::string("\033[33m")+x+std::string("\033[0m")
#endif



namespace Utility
{
	void CreateRotationGenericMatrix(glm::mat4 &mat, float angle, const glm::vec3& axis);

	void CreateTranslationMatrix(glm::mat4 &mat, glm::vec3 vec);

	glm::mat4 CreateOrthoMatrix(float width, float height, float nearVal, float farVal);

	glm::mat4 CreatePerspectiveMatrix(float fovy, float aspect, float nearVal, float farVal);

	void NormalizeAngle(float& value);

	bool GetPathToRes(std::string& resString);
}
