#pragma once

#include "glm/glm.hpp"

class Camera
{
private:
	glm::vec3 m_EyeVec, m_CenterVec, m_UpVec;

public:
	Camera(const glm::vec3& _EyeVec, const glm::vec3& _CenterVec, const glm::vec3& _UpVec);
	~Camera();

	glm::mat4 m_ViewMatrix;
	void CreateViewMatrix();
	void RotateViewHorizontal(float amount);
	void RotateViewVertical(float amount);
	const glm::vec3& GetEyePos() const;
};