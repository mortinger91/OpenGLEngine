#pragma once

#include <memory>
#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

//TODO: simplify construction defining a builder class
class Mesh
{
private:
	std::string m_Name;
	std::vector <glm::vec3> m_VerticesPositions;
	std::vector <glm::vec3> m_VerticesColors;
	std::vector <glm::vec3> m_VerticesNormals;
	std::vector <glm::vec2> m_VerticesTexCoords;
	std::vector <unsigned int> m_VerticesIndices;

	float* arrayV;
	unsigned int* arrayI;

	void ConvertVectorsToArray(unsigned int& sizeV, unsigned int& sizeI);
	void parse(const char * filepath);
	void init();
	void CreateScalingMatrix(glm::mat4 &mat, float scale);

public:
	Mesh(const std::string& name, std::vector <glm::vec3> verticesPositions_, std::vector <glm::vec3> verticesColors_, std::vector <glm::vec3> verticesNormals_, std::vector <glm::vec2> verticesTexCoords_, std::vector <unsigned int> verticesIndices_);
	Mesh(const std::string& name, const std::string& filepath);
	// deleted copy constructor
	Mesh(const Mesh& mesh) = delete;
	Mesh(Mesh&& mesh) noexcept;
	~Mesh();

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::shared_ptr<Material> m_Material;

	glm::vec3 m_TranslationVec;
	glm::vec3 m_RotationVec;
	float m_Scale;
	glm::mat4 m_CustomTransform;

	void SetMaterial(std::shared_ptr<Material> mat);

	glm::mat4 m_ModelMatrix;
	void CreateModelMatrix(const glm::vec3& ModelTranslationVec, const glm::vec3& ModelRotationVec, float ModelScale);

	void Bind() const;
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projMatrix, const glm::vec3& ModelTranslationVec, const glm::vec3& ModelRotationVec, float ModelScale);
};