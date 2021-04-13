#pragma once

class Utility
{
public:
	//void NormalizeVector(glm::vec3 &vec)
	//{
	//	//float sqrt = glm::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	//	//if (sqrt == 0) return;
	//	//vec.x /= sqrt;
	//	//vec.y /= sqrt;
	//	//vec.z /= sqrt;
	//	vec = glm::normalize(vec);
	//}

	//void ConvertToRadians(float &value)
	//{
	//	//float degrees = (angle * glm::pi<float>()) / 180.f;
	//	value = glm::radians(value);
	//}

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

	static void CreateRotationGenericMatrix(glm::mat4 &mat, float angle, const glm::vec3& axis)
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

	static void CreateTranslationMatrix(glm::mat4 &mat, glm::vec3 vec)
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
};