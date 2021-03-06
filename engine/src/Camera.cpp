#include "Camera.h"
#include "Utility.h"

Camera::Camera(const glm::vec3& _EyeVec, const glm::vec3& _CenterVec, const glm::vec3& _UpVec)
	: m_EyeVec(_EyeVec),
	  m_CenterVec(_CenterVec),
	  m_UpVec(_UpVec),
	  m_CameraMoved(false)
{
	CreateViewMatrix();
}

Camera::~Camera()
{

}

const glm::mat4& Camera::GetViewMatrix()
{
	if (m_CameraMoved)
		CreateViewMatrix();
	return m_ViewMatrix;
}

void Camera::RotateViewHorizontal(float amount)
{
	glm::mat4 matRotation;
	Utility::CreateRotationGenericMatrix(matRotation, amount, m_UpVec);
	m_EyeVec = glm::mat3(matRotation) * m_EyeVec;
	m_CameraMoved = true;
}

void Camera::RotateViewVertical(float amount)
{
	glm::mat4 matRotation;
	Utility::CreateRotationGenericMatrix(matRotation, amount, glm::normalize(glm::cross(m_UpVec, glm::normalize(m_EyeVec)))
	);
	m_EyeVec = glm::mat3(matRotation) * m_EyeVec;
	m_UpVec = glm::mat3(matRotation) * m_UpVec;
	m_CameraMoved = true;
}

void Camera::MoveEyePosition(glm::vec3 amount)
{
	m_EyeVec = amount + m_EyeVec;
	m_CameraMoved = true;
}

const glm::vec3& Camera::GetEyePos() const
{
	return m_EyeVec;
}

void Camera::CreateViewMatrix()
{
	glm::vec3 w = glm::normalize(m_EyeVec - m_CenterVec);
	glm::vec3 u = glm::normalize(glm::cross(m_UpVec, w));
	glm::vec3 v = glm::cross(w, u);

	m_ViewMatrix[0][0] = u.x;
	m_ViewMatrix[1][0] = u.y;
	m_ViewMatrix[2][0] = u.z;
	m_ViewMatrix[3][0] = -glm::dot(u, m_EyeVec); //-u.x*m_EyeVec.x - u.y*m_EyeVec.y - u.z*m_EyeVec.z;
	m_ViewMatrix[0][1] = v.x;
	m_ViewMatrix[1][1] = v.y;
	m_ViewMatrix[2][1] = v.z;
	m_ViewMatrix[3][1] = -glm::dot(v, m_EyeVec); //-v.x*m_EyeVec.x - v.y*m_EyeVec.y - v.z*m_EyeVec.z;
	m_ViewMatrix[0][2] = w.x;
	m_ViewMatrix[1][2] = w.y;
	m_ViewMatrix[2][2] = w.z;
	m_ViewMatrix[3][2] = -glm::dot(w, m_EyeVec); //-w.x*m_EyeVec.x - w.y*m_EyeVec.y - w.z*m_EyeVec.z;
	m_ViewMatrix[0][3] = 0;
	m_ViewMatrix[1][3] = 0;
	m_ViewMatrix[2][3] = 0;
	m_ViewMatrix[3][3] = 1;
}
