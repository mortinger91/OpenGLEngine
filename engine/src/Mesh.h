#pragma once

#include <memory>
#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "Material.h"

// forward declarations
// class VertexBuffer;
// class VertexArray;
// class IndexBuffer;
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
//class Material;

class Mesh
{
private:
	std::string m_Name;
	std::vector <glm::vec3> verticesPositions;
	std::vector <glm::vec3> verticesColors;
	std::vector <glm::vec3> verticesNormals;
	std::vector <glm::vec2> verticesTexCoords;
	std::vector <unsigned int> verticesIndices;

	void ConvertVectorsToArray(unsigned int& sizeV, unsigned int& sizeI);
	void parse(const char * filepath);
	void init();
	void CreateScalingMatrix(glm::mat4 &mat, float scale);

public:
	Mesh(const std::string& name, std::vector <glm::vec3> verticesPositions_, std::vector <glm::vec3> verticesColors_, std::vector <glm::vec3> verticesNormals_, std::vector <glm::vec2> verticesTexCoords_, std::vector <unsigned int> verticesIndices_);
	//Mesh(const std::string& name, const char* filepath);
	Mesh(const std::string& name, const std::string& filepath);
	// deleted copy constructor
	Mesh(const Mesh& mesh) = delete;
	Mesh(Mesh&& mesh) noexcept;
	~Mesh();

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Material> m_Material;

	float* arrayV;
	unsigned int* arrayI;

	glm::vec3 m_TranslationVec;
	glm::vec3 m_RotationVec;
	float m_Scale;
	bool m_UseTexture;

	glm::mat4 m_ModelMatrix;
	void CreateModelMatrix();

	void SetMaterial(const std::string& texturePath, float shininess, float specular, float diffuse, float ambient); 

	void Bind() const;
};