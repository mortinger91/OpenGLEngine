#pragma once

#include "GL/glew.h"

class VertexArray;
class IndexBuffer;
class Shader;

// il __ fa capire che questa funzione è specifica del compilatore, msvc in questo caso
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	//Renderer();
	//~Renderer();
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};