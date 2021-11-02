#include "Model.h"

Model::Model(const std::string& name)
	: m_Name(name),
	  m_TranslationVec(0.f,0.f,0.f),
	  m_RotationVec(0.f, 0.f, 0.f),
	  m_Scale(1.f),
	  m_UseTextures(true)
{
	
}

Model::Model(const std::string& name, const Model& model)
	: m_Name(name),
	  m_TranslationVec(model.m_TranslationVec),
	  m_RotationVec(model.m_RotationVec),
	  m_Scale(model.m_Scale),
	  m_UseTextures(model.m_UseTextures),
	  m_Meshes(model.m_Meshes)
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
		mesh->Draw(viewMatrix, projMatrix, m_TranslationVec, m_RotationVec, m_Scale, m_UseTextures);
	}
}

std::string Model::GetName() const
{
	return m_Name;
}
