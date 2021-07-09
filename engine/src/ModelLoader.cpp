#include "ModelLoader.h"
#include "Utility.h"

#include <future>
#include <chrono>
#include <iostream>
#include <string>

// testing parameter
//#define SINGLECORE

std::unique_ptr<Mesh> ModelLoader::LoadMesh(const aiScene* scene, unsigned int meshIndex)
{
	#ifdef DEBUG
		long double start;
		long double passed;
		start = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	#endif

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<unsigned int> indices;

	ASSERT(scene->HasMeshes())
	bool getUVs = false;
	if (scene->mMeshes[meshIndex]->GetNumUVChannels() == 1 && scene->mMeshes[meshIndex]->mNumUVComponents[0] == 2)
	{
		getUVs = true;
	}

	for (unsigned int i = 0; i < scene->mMeshes[meshIndex]->mNumVertices; ++i)
	{
		ASSERT(scene->mMeshes[meshIndex]->HasNormals())
		normals.push_back(
			glm::vec3(
				scene->mMeshes[meshIndex]->mNormals[i].x, 
				scene->mMeshes[meshIndex]->mNormals[i].y, 
				scene->mMeshes[meshIndex]->mNormals[i].z));
		vertices.push_back(
			glm::vec3(scene->mMeshes[meshIndex]->mVertices[i].x, 
				scene->mMeshes[meshIndex]->mVertices[i].y, 
				scene->mMeshes[meshIndex]->mVertices[i].z));
		if (getUVs)
		{
			texcoords.push_back(glm::vec2(scene->mMeshes[meshIndex]->mTextureCoords[0][i].x, scene->mMeshes[meshIndex]->mTextureCoords[0][i].y));
		}
		else
		{
			texcoords.push_back(glm::vec2(0.f, 0.f));
		}
	}

	for (unsigned int i = 0; i < scene->mMeshes[meshIndex]->mNumFaces; ++i)
	{
		ASSERT(scene->mMeshes[meshIndex]->HasFaces())
		ASSERT(scene->mMeshes[meshIndex]->mFaces->mNumIndices == 3)
		indices.push_back(scene->mMeshes[meshIndex]->mFaces[i].mIndices[0]);
		indices.push_back(scene->mMeshes[meshIndex]->mFaces[i].mIndices[1]);
		indices.push_back(scene->mMeshes[meshIndex]->mFaces[i].mIndices[2]);
	}

	#ifdef DEBUG
	passed = (std::chrono::high_resolution_clock::now().time_since_epoch().count() - start) / 1000000;
	std::cout << "LoadMesh of meshIndex: " + std::to_string(meshIndex) + ": " + std::to_string(passed) + " ms\n";
	#endif

	// TODO: create a mesh constructor(adapter class?) that takes assimp vectors as input and convert them to buffer data without calling "ConvertVectorsToArray"
	//return std::make_unique<Mesh>("mesh_"+std::to_string(meshIndex), scene->mMeshes[meshIndex]->mVertices, scene->mMeshes[meshIndex]->mNormals, scene->mMeshes[meshIndex]->mNumVertices, scene->mMeshes[meshIndex]->mFaces, scene->mMeshes[meshIndex]->mNumFaces, true);
	return std::make_unique<Mesh>("mesh_" + std::to_string(meshIndex), vertices, normals, texcoords, indices, true);
}

bool ModelLoader::LoadModel(Model& model, const std::string& pFile, std::vector<std::shared_ptr<Material>> mats, std::vector<unsigned int> matIndexes)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;
	
	#ifdef DEBUG
		long double start;
		long double passed;
		start = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	#endif

	// And have it read the given file with some example postprocessing
	// Very long operation, depend on hdd speed: 5.1secs on slow hdd and 1.2secs on nvme
	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType
	);
	#ifdef DEBUG
	passed = (std::chrono::high_resolution_clock::now().time_since_epoch().count() - start) / 1000000;
		std::cout << REDTEXT("   ReadFile of model: " + model.GetName() + ": " + std::to_string(passed) + " ms\n");
	#endif

	#ifdef DEBUG
		start = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	#endif

	// If the import failed, report it
 	if( !scene) 
	{
		std::cout << REDTEXT(importer.GetErrorString());
		std::cout << std::endl;
		return false;
	}

	ASSERT(matIndexes.size() == scene->mNumMeshes);

	std::vector<std::future<std::unique_ptr<Mesh>>> loadingMeshes;
	
	for (int m = 0; m < scene->mNumMeshes; m++)
	{
		std::future<std::unique_ptr<Mesh>> loadingMesh = std::async(std::launch::async, &ModelLoader::LoadMesh, scene, m);
		loadingMeshes.push_back(std::move(loadingMesh));
		#ifdef SINGLECORE
			std::unique_ptr<Mesh> ptr = loadingMeshes[m].get();
			ptr->MakeVertexArray();
			ptr->SetMaterial(mats[matIndexes[m]]);
			model.MoveMesh(std::move(ptr));
		#endif
	}

	#ifndef SINGLECORE
		for (int m = 0; m < scene->mNumMeshes; m++)
		{
			std::unique_ptr<Mesh> ptr = loadingMeshes[m].get();
			ptr->MakeVertexArray();
			ptr->SetMaterial(mats[matIndexes[m]]);
			model.MoveMesh(std::move(ptr));
		}
	#endif
	
	// Now we can access the file's contents.
	//DoTheSceneProcessing( scene);

	#ifdef DEBUG
		passed = (std::chrono::high_resolution_clock::now().time_since_epoch().count() - start) / 1000000;
		std::cout << REDTEXT("   LoadModel excluded ReadFile of model: " + model.GetName() + ": " + std::to_string(passed) + " ms\n");
	#endif

	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}