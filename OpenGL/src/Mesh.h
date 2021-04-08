#pragma once

#include <memory>
#include <vector>
#include "glm/glm.hpp"

// forward declarations
class VertexBuffer;
class VertexArray;
class IndexBuffer;

class Mesh
{
private:
	std::vector <glm::vec3> verticesPositions;
	std::vector <glm::vec3> verticesColors;
	std::vector <glm::vec3> verticesNormals;
	std::vector <glm::vec2> verticesTexCoords;
	std::vector <unsigned int> verticesIndices;

	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	

	float* arrayV;
	unsigned int* arrayI;

	//void ConvertVectorsToArray(float[] verticesArray, int& sizeV, unsigned int[] indexArray, int& sizeI);
	void ConvertVectorsToArray(int& sizeV, int& sizeI);
	void parse(const char * filepath);
	void init();

public:
	Mesh(std::vector <glm::vec3> verticesPositions_, std::vector <glm::vec3> verticesColors_, std::vector <glm::vec3> verticesNormals_, std::vector <glm::vec2> verticesTexCoords_, std::vector <unsigned int> verticesIndices_);
	Mesh(const char * filepath);
	~Mesh();

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;

	//VertexArray GetVao();
	//IndexBuffer GetIndexBuffer();

};