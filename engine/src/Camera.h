#pragma once

#include "glm/glm.hpp"

class Camera
{
private:
	glm::vec3 m_EyeVec, m_CenterVec, m_UpVec;
	glm::mat4 m_ViewMatrix;
	bool m_CameraMoved;

	void CreateViewMatrix();

public:
	Camera(const glm::vec3& _EyeVec, const glm::vec3& _CenterVec, const glm::vec3& _UpVec);
	~Camera();

	const glm::mat4& GetViewMatrix();
	void RotateViewHorizontal(float amount);
	void RotateViewVertical(float amount);
	void MoveEyePosition(glm::vec3 amount);
	const glm::vec3& GetEyePos() const;
};
