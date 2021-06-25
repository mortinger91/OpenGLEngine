#include "Model.h"

Model::Model(const std::string& name)
	: m_Name(std::move(name)),
	  m_TranslationVec(0.f,0.f,0.f),
	  m_RotationVec(0.f, 0.f, 0.f),
	  m_Scale(1.f)
{
	
}

// moves the mesh inside the mesh vector
void Model::MoveMesh(const Mesh&& mesh) noexcept
{
	m_Meshes.push_back(mesh);
}

void Model::Draw(const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
{
	for (Mesh& mesh : m_Meshes)
	{
		mesh.Draw(viewMatrix, projMatrix);
	}
}