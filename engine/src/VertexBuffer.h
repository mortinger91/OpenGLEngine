#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	// size for the bytes of the buffer
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();
	void Bind() const;
	void Unbind() const;
};
