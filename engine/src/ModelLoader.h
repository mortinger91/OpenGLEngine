#pragma once

#include "Model.h"
#include "Mesh.h"
#include "Material.h"

class ModelLoader
{
private:
	static std::unique_ptr<Mesh> LoadMesh(const aiScene* scene, unsigned int meshIndex);
public:
	static bool LoadModel(Model& model, const std::string& pFile, std::vector<std::shared_ptr<Material>> mats, std::vector<unsigned int> matIndexes);
};
