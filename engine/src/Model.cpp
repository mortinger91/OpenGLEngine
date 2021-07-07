#include "Model.h"

Model::Model(const std::string& name)
	: m_Name(std::move(name)),
	  m_TranslationVec(0.f,0.f,0.f),
	  m_RotationVec(0.f, 0.f, 0.f),
	  m_Scale(1.f)
{
	
}

// moves the mesh inside the mesh vector
void Model::MoveMesh(std::unique_ptr<Mesh> mesh) noexcept
{
	m_Meshes.push_back(std::move(mesh));
}

void Model::Draw(const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
{
	for (auto& mesh : m_Meshes)
	{
		mesh->Draw(viewMatrix, projMatrix, m_TranslationVec, m_RotationVec, m_Scale);
	}
}

std::string Model::GetName() const
{
	return m_Name;
}