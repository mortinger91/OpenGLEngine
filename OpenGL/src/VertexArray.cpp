#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

// disable warning (const void*)(offset) dynamic cast, only for MSVC(Visual Studio)
#pragma warning( push )
#pragma warning( disable : 4312 )

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		// enable an attribute for the vertices in the vertex buffer
		GLCall(glEnableVertexAttribArray(i));
		// setup of the attribute "position"
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)(offset)));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

#pragma warning( pop )

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

