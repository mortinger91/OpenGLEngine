#include "Mesh.h"

#include <iostream>
#include <string>

#include "VertexBufferLayout.h"
#include "Utility.h"
#include "Renderer.h"
#include "Shader.h"
#include "Channel.h"

Mesh::Mesh(const std::string& name, std::vector <glm::vec3> verticesPositions_, std::vector <glm::vec3> verticesNormals_, std::vector <glm::vec2> verticesTexCoords_, std::vector <unsigned int> verticesIndices_)
	: m_Name(name),
	  m_VerticesPositions(verticesPositions_),
	  m_VerticesNormals(verticesNormals_),
	  m_VerticesTexCoords(verticesTexCoords_),
	  m_VerticesIndices(verticesIndices_),
	  sizeV(0),
	  sizeI(0),
	  isLoaded(false)
{
	#ifdef DEBUG
		//std::cout << "Called default mesh constructor of: " + name + "\n";
	#endif
	init();
}

// only used for teapot object, deprecated
Mesh::Mesh(const std::string& name, const std::string& filepath)
	: m_Name(name),
	  isLoaded(false)
{
	#ifdef DEBUG
		//std::cout << "Called mesh constructor from file of: " << m_Name << std::endl;
	#endif
	parse(filepath.c_str());
	init();
}

Mesh::Mesh(const std::string& name, aiVector3D* vertices, aiVector3D* normals, unsigned int sizeVertices, aiFace* indices, unsigned int sizeIndices)
	: m_Name(name),
	  isLoaded(false)
{



}

// move constructor
Mesh::Mesh(Mesh&& mesh) noexcept
	: m_Name(mesh.m_Name),
	  m_VAO(std::move(mesh.m_VAO)),
	  m_VertexBuffer(std::move(mesh.m_VertexBuffer)),
	  m_IndexBuffer(std::move(mesh.m_IndexBuffer)),
	  m_Material(std::move(mesh.m_Material)),
	  m_TranslationVec(mesh.m_TranslationVec),
	  m_RotationVec(mesh.m_RotationVec),
	  m_Scale(mesh.m_Scale),
	  m_CustomTransform(mesh.m_CustomTransform),
	  arrayV(mesh.arrayV),
	  arrayI(mesh.arrayI),
	  isLoaded(mesh.isLoaded)
{
	mesh.arrayV = nullptr;
	mesh.arrayI = nullptr;
	#ifdef DEBUG
		//std::cout << "Called mesh move constructor of:" << m_Name << std::endl;
	#endif
}

Mesh::~Mesh()
{
	free(arrayV);
	free(arrayI);
	#ifdef DEBUG
		//std::cout << "Called mesh destructor of:" << m_Name << std::endl;
	#endif
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

	std::string pathString = std::string(filepath);
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
			m_VerticesPositions.push_back(glm::vec3(x, y, z));
			m_VerticesTexCoords.push_back(glm::vec2(0.f,0.f));

			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
		else if ((c1 == 'v') && (c2 == 'n')) 
		{
			fscanf(fp, "%f %f %f", &x, &y, &z);
			m_VerticesNormals.push_back(glm::normalize(glm::vec3(x, y, z)));
		}
		else if (c1 == 'f')
		{
			fscanf(fp, "%d//%d %d//%d %d//%d", &fx, &ignore, &fy, &ignore, &fz, &ignore);
			m_VerticesIndices.push_back(fx - 1);
			m_VerticesIndices.push_back(fy - 1);
			m_VerticesIndices.push_back(fz - 1);
		}
	}

	fclose(fp); // Finished parsing
	// Recenter the teapot
	float avgY = (minY + maxY) / 2.0f - 0.0234f;
	float avgZ = (minZ + maxZ) / 2.0f;
	for (unsigned int i = 0; i < m_VerticesPositions.size(); ++i)
	{
		glm::vec3 shiftedVertex = (m_VerticesPositions[i] - glm::vec3(0.0f, avgY, avgZ)) * glm::vec3(0.975f, 0.975f, 0.975f);
		m_VerticesPositions[i] = shiftedVertex;
	}
}

void Mesh::ConvertVectorsToArray(unsigned int& sizeV, unsigned int& sizeI)
{
	sizeV = static_cast<int>(m_VerticesPositions.size()) * 11;
	arrayV = (float*)malloc(sizeof(float) * sizeV);
	sizeI = static_cast<int>(m_VerticesIndices.size());
	arrayI = (unsigned int*)malloc(sizeof(unsigned int) * sizeI);

	int index = 0;
	for (unsigned int i = 0; i < m_VerticesPositions.size(); ++i)
	{
		arrayV[index++] = m_VerticesPositions[i].x;
		arrayV[index++] = m_VerticesPositions[i].y;
		arrayV[index++] = m_VerticesPositions[i].z;

		arrayV[index++] = m_VerticesNormals[i].x;
		arrayV[index++] = m_VerticesNormals[i].y;
		arrayV[index++] = m_VerticesNormals[i].z;

		arrayV[index++] = m_VerticesTexCoords[i].x;
		arrayV[index++] = m_VerticesTexCoords[i].y;
	}
	
	index = 0;
	for (unsigned int i = 0; i < m_VerticesIndices.size(); ++i)
	{
		arrayI[index++] = m_VerticesIndices[i];
	}
}

void Mesh::init()
{
	#ifdef DEBUG
		long double start;
		long double passed;
		start = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	#endif

	m_TranslationVec = glm::vec3(0.f, 0.f, 0.f);
	m_RotationVec = glm::vec3(0.f, 0.f, 0.f);
	m_Scale = 1.f;
	m_CustomTransform = glm::mat4(1.f);

	ConvertVectorsToArray(sizeV, sizeI);

	#ifdef DEBUG
	passed = (std::chrono::high_resolution_clock::now().time_since_epoch().count() - start) / 1000000;
		std::cout << "      init of mesh: " + m_Name + ": " + std::to_string(passed) + " ms\n";
	#endif
}

void Mesh::MakeVertexArray()
{
	// initializing vertex buffer layout
	VertexBufferLayout layout;
	// position of the vertices of the two triangles making up a square
	// layout(location = 0) in vertex shader
	// vertices
	layout.Push<float>(3);
	// normals
	layout.Push<float>(3);
	// texture UV coordinates
	layout.Push<float>(2);

	// these operations must be done on main thread, context cannot be accessed in other threads
	// initializing vertex array
	m_VAO = std::make_unique<VertexArray>();

	// initializing vertex buffer
	m_VertexBuffer = std::make_unique<VertexBuffer>(static_cast<const void*>(arrayV), static_cast<unsigned int>(sizeV * sizeof(float)));

	// adding vertex buffer and layout to vertex array
	m_VAO->AddBuffer(*m_VertexBuffer, layout);

	// creating an index buffer
	m_IndexBuffer = std::make_unique<IndexBuffer>(arrayI, sizeI);

	isLoaded = true;
}

void Mesh::CreateScalingMatrix(glm::mat4 &mat, float scale)
{
	mat = glm::mat4(0.f);
	mat[0][0] = scale;
	mat[1][1] = scale;
	mat[2][2] = scale;
	mat[3][3] = 1;
}

void Mesh::SetMaterial(std::shared_ptr<Material> mat)
{
	m_Material = mat;
}

void Mesh::CreateModelMatrix(const glm::vec3& ModelTranslationVec, const glm::vec3& ModelRotationVec, float ModelScale)
{
	glm::mat4 matTranslate;
	glm::mat4 matRotateZ;
	glm::mat4 matRotateY;
	glm::mat4 matRotateX;
	glm::mat4 matScale;

	Utility::CreateTranslationMatrix(matTranslate, m_TranslationVec+ModelTranslationVec);

	// TODO: use quaternions to perform model rotation in order to avoid gimbal lock
	Utility::CreateRotationGenericMatrix(matRotateZ, m_RotationVec.z+ModelRotationVec.z, glm::vec3(0,0,1));
	Utility::CreateRotationGenericMatrix(matRotateY, m_RotationVec.y+ModelRotationVec.y, glm::vec3(0,1,0));
	Utility::CreateRotationGenericMatrix(matRotateX, m_RotationVec.x+ModelRotationVec.x, glm::vec3(1,0,0));

	CreateScalingMatrix(matScale, m_Scale*ModelScale);

	m_ModelMatrix = matTranslate * matRotateY * matRotateZ * matRotateX * matScale;
}

void Mesh::Bind() const
{
	m_VAO->Bind();
	m_IndexBuffer->Bind();
	m_Material->Bind();
}

void Mesh::Draw(const glm::mat4& viewMatrix, const glm::mat4& projMatrix, const glm::vec3& ModelTranslationVec, const glm::vec3& ModelRotationVec, float ModelScale, bool UseTextures)
{
	if (isLoaded)
	{ 
		Renderer renderer;
		Bind();

		glm::mat4 mv;
		glm::mat4 mvp;

		CreateModelMatrix(ModelTranslationVec, ModelRotationVec, ModelScale);
		m_ModelMatrix = m_CustomTransform * m_ModelMatrix;

		mv = viewMatrix * m_ModelMatrix;
		mvp = projMatrix * mv;

		m_Material->m_Shader->SetUniformMat4f("u_MV", mv);
		m_Material->m_Shader->SetUniformMat4f("u_MVP", mvp);
		m_Material->m_Shader->SetUniform1i("useTexture",UseTextures);
		// TODO: create a batch renderer:
			// calling draw does not draw the object but add it in a queue
			// another command is implemented to render the whole frame
			// all the meshes that shares the same materials, shaders, uniforms are added together
			// in one big vertex array, that is rendered using one draw call
		renderer.Draw(*this, *m_Material->m_Shader);
	}
}