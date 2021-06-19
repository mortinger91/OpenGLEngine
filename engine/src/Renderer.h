#pragma once

#include "glew.h"

class VertexArray;
class IndexBuffer;
class Shader;
class Mesh;

// __ means that the function is compiler intrinsic, so it's specific to msvc, other compilers will use different functions
#ifdef _WIN32
	#define ASSERT(x) if(!(x)) __debugbreak();
	#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#elif __linux__
	#define ASSERT(x) if(!(x)) ;
	#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#endif

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	//Renderer();
	//~Renderer();
	void Clear() const;
	void Draw(const Mesh& mesh, const Shader& shader) const;
};