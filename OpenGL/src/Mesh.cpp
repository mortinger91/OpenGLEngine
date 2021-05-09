#pragma once

#include <iostream>
#include <string>
#include "Mesh.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Utility.h"

Mesh::Mesh(const std::string& name, std::vector <glm::vec3> verticesPositions_, std::vector <glm::vec3> verticesColors_, std::vector <glm::vec3> verticesNormals_, std::vector <glm::vec2> verticesTexCoords_, std::vector <unsigned int> verticesIndices_) 
	: m_Name(name),
	  verticesPositions(verticesPositions_),
	  verticesColors(verticesColors_),
	  verticesNormals(verticesNormals_),
	  verticesTexCoords(verticesTexCoords_),
	  verticesIndices(verticesIndices_)
{
	//std::cout << "Called mesh constructor from vectors of:" << m_Name << std::endl;
	init();
}

Mesh::Mesh(const std::string& name, const char * filepath)
	: m_Name(name)
{
	//std::cout << "Called mesh constructor from file of:" << m_Name << std::endl;
	parse(filepath);
	init();
}

// copy constructor
Mesh::Mesh(const Mesh& mesh)
	: m_Name(mesh.m_Name),
	  verticesPositions(mesh.verticesPositions),
	  verticesColors(mesh.verticesColors),
	  verticesNormals(mesh.verticesNormals),
	  verticesTexCoords(mesh.verticesTexCoords),
	  verticesIndices(mesh.verticesIndices)
{
	std::cout << "Called mesh copy constructor of:" << m_Name << std::endl;
	init();
}

// move constructor
Mesh::Mesh(Mesh&& mesh) noexcept
	: m_Name(mesh.m_Name),
	  verticesPositions(mesh.verticesPositions),
	  verticesColors(mesh.verticesColors),
	  verticesNormals(mesh.verticesNormals),
	  verticesTexCoords(mesh.verticesTexCoords),
	  verticesIndices(mesh.verticesIndices),
	  m_VAO(std::move(mesh.m_VAO)),
	  m_VertexBuffer(std::move(mesh.m_VertexBuffer)),
	  m_IndexBuffer(std::move(mesh.m_IndexBuffer)),
	  m_Material(std::move(mesh.m_Material)),
	  m_TranslationVec(mesh.m_TranslationVec),
	  m_RotationVec(mesh.m_RotationVec),
	  m_Scale(mesh.m_Scale),
	  arrayV(mesh.arrayV),
	  arrayI(mesh.arrayI)
{
	mesh.arrayV = nullptr;
	mesh.arrayI = nullptr;
	//std::cout << "Called mesh move constructor of:" << m_Name << std::endl;
}

Mesh::~Mesh()
{
	free(arrayV);
	free(arrayI);
	//std::cout << "Called mesh destructor of:" << m_Name << std::endl;
}

// OBJ file parser function. Used for loading the teapot.obj file.
void Mesh::parse(const char * filepath)
{
	FILE* fp;
	float x, y, z;
	int fx, fy, fz, ignore;
	int c1, c2;
	float minY = INFINITY, minZ = INFINITY;
	float maxY = -INFINITY, maxZ = -INFINITY;

	fp = fopen(filepath, "rb");

	if (fp == NULL) 
	{
		std::cerr << "Error loading file: " << filepath << std::endl;
		//std::getchar();
		exit(-1);
	}

	while (!feof(fp)) 
	{
		c1 = fgetc(fp);
		while (!(c1 == 'v' || c1 == 'f')) 
		{
			c1 = fgetc(fp);
			if (feof(fp))
				break;
		}
		c2 = fgetc(fp);

		if ((c1 == 'v') && (c2 == ' ')) 
		{
			fscanf(fp, "%f %f %f", &x, &y, &z);
			verticesPositions.push_back(glm::vec3(x, y, z));

			verticesColors.push_back(glm::vec3(0.8f,0.3f,0.3f)); 
			verticesTexCoords.push_back(glm::vec2(0.f,0.f));

			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
		else if ((c1 == 'v') && (c2 == 'n')) 
		{
			fscanf(fp, "%f %f %f", &x, &y, &z);
			verticesNormals.push_back(glm::normalize(glm::vec3(x, y, z)));
		}
		else if (c1 == 'f')
		{
			fscanf(fp, "%d//%d %d//%d %d//%d", &fx, &ignore, &fy, &ignore, &fz, &ignore);
			verticesIndices.push_back(fx - 1);
			verticesIndices.push_back(fy - 1);
			verticesIndices.push_back(fz - 1);
		}
	}

	fclose(fp); // Finished parsing
	// Recenter the teapot
	float avgY = (minY + maxY) / 2.0f - 0.0234f;
	float avgZ = (minZ + maxZ) / 2.0f;
	for (unsigned int i = 0; i < verticesPositions.size(); ++i) 
	{
		glm::vec3 shiftedVertex = (verticesPositions[i] - glm::vec3(0.0f, avgY, avgZ)) * glm::vec3(0.975f, 0.975f, 0.975f);
		verticesPositions[i] = shiftedVertex;
	}
}

void Mesh::ConvertVectorsToArray(int& sizeV, int& sizeI)
{
	sizeV = verticesPositions.size() * 11;
	arrayV = (float*)malloc(sizeof(float) * sizeV);
	sizeI = verticesIndices.size();
	arrayI = (unsigned int*)malloc(sizeof(unsigned int) * sizeI);

	int index = 0;
	for (unsigned int i = 0; i < verticesPositions.size(); ++i)
	{
		arrayV[index++] = verticesPositions[i].x;
		arrayV[index++] = verticesPositions[i].y;
		arrayV[index++] = verticesPositions[i].z;

		arrayV[index++] = verticesColors[i].x;
		arrayV[index++] = verticesColors[i].y;
		arrayV[index++] = verticesColors[i].z;

		arrayV[index++] = verticesNormals[i].x;
		arrayV[index++] = verticesNormals[i].y;
		arrayV[index++] = verticesNormals[i].z;

		arrayV[index++] = verticesTexCoords[i].x;
		arrayV[index++] = verticesTexCoords[i].y;
	}
	
	index = 0;
	for (unsigned int i = 0; i < verticesIndices.size(); ++i)
	{
		arrayI[index++] = verticesIndices[i];
	}
}

void Mesh::init()
{
	m_TranslationVec = glm::vec3(0.f, 0.f, 0.f);
	m_RotationVec = glm::vec3(0.f, 0.f, 0.f);
	m_Scale = 1.f;

	// initializing vertex array
	m_VAO = std::make_unique<VertexArray>();

	int sizeV;
	int sizeI;
	//ConvertVectorsToArray(sizeV, sizeI, arrayV, arrayI);
	ConvertVectorsToArray(sizeV, sizeI);

	// initializing vertex buffer
	m_VertexBuffer = std::make_unique<VertexBuffer>(arrayV, sizeV * sizeof(float));
	
	// initializing vertex buffer layout
	VertexBufferLayout layout;
	// position of the vertices of the two triangles making up a square
	// layout(location = 0) in vertex shader
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	// adding vertex buffer and layout to vertex array
	m_VAO->AddBuffer(*m_VertexBuffer, layout);

	// creating an index buffer
	m_IndexBuffer = std::make_unique<IndexBuffer>(arrayI, sizeI);
}

void Mesh::CreateScalingMatrix(glm::mat4 &mat, float scale)
{
	mat = glm::mat4(0.f);
	mat[0][0] = scale;
	mat[1][1] = scale;
	mat[2][2] = scale;
	mat[3][3] = 1;
}

void Mesh::CreateModelMatrix()
{
	glm::mat4 matTranslate;
	glm::mat4 matRotateZ;
	glm::mat4 matRotateY;
	glm::mat4 matRotateX;
	glm::mat4 matScale;

	Utility::CreateTranslationMatrix(matTranslate, m_TranslationVec);

	// TODO: use quaternions to perform model rotation in order to avoid gimbal lock
	Utility::CreateRotationGenericMatrix(matRotateZ, m_RotationVec.z, glm::vec3(0,0,1));
	Utility::CreateRotationGenericMatrix(matRotateY, m_RotationVec.y, glm::vec3(0,1,0));
	Utility::CreateRotationGenericMatrix(matRotateX, m_RotationVec.x, glm::vec3(1,0,0));

	CreateScalingMatrix(matScale, m_Scale);

	m_ModelMatrix = matTranslate * matRotateY * matRotateZ * matRotateX * matScale;
}

void Mesh::SetMaterial(const std::string& texturePath, float shininess, float specular, float diffuse, float ambient)
{
	m_Material = std::make_unique<Material>(texturePath, shininess, specular, diffuse, ambient);
}

void Mesh::Bind() const
{
	m_VAO->Bind();
	m_IndexBuffer->Bind();
	m_Material->Bind();
}
