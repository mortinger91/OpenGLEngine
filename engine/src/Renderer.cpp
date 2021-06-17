#include <iostream>
#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Mesh.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	GLenum error;
	while (error = glGetError())
	{
		std::cout << "OpenGL Error: (" <<  error << ") " <<
			function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	// state using function
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

//void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
void Renderer::Draw(const Mesh& mesh, const Shader& shader) const
{
	mesh.Bind();
	shader.Bind();
	// DRAW CALL: render primitives from array data
	GLCall(glDrawElements(GL_TRIANGLES, mesh.m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
}