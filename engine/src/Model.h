#pragma once

#include <vector>
#include <memory>
#include "Mesh.h"
#include "Shader.h"

class Model
{
public:
	Model(const std::string& name);
	Model(const std::string& name, const Model& model);
	void MoveMesh(std::unique_ptr<Mesh> mesh) noexcept;
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projMatrix);
	std::string GetName() const;

	// these vector are applied to all the meshes composing the model, they are the actual model matrix of the whole model
	glm::vec3 m_TranslationVec;
	glm::vec3 m_RotationVec;
	float m_Scale;
	std::vector<std::shared_ptr<Mesh>> m_Meshes;

	bool m_UseTextures;
private:
	std::string m_Name;
	// the translation, rotation and scale vector of the meshes are relative to the model center, they perform the mesh->model transformation
};