namespace Plane
{
	// 3 x position, 3 x color, 3 x normal, 2 x texture coordinates
	std::vector<glm::vec3> positions =
	{
		// bot
		glm::vec3( 1.0f,  0.0f,   1.0f),
		glm::vec3(-1.0f,  0.0f,   1.0f),
		glm::vec3( 1.0f,  0.0f,  -1.0f),
		glm::vec3(-1.0f,  0.0f,  -1.0f)
	};

	std::vector<glm::vec3> colors =
	{           
		glm::vec3(0.5f,  0.3f,  0.1f),    
		glm::vec3(0.5f,  0.3f,  0.1f),    
		glm::vec3(0.5f,  0.3f,  0.1f),    
		glm::vec3(0.5f,  0.3f,  0.1f)
	};

	std::vector<glm::vec3> normals =
	{
		glm::vec3(0.0f, 1.0f,  0.0f),
		glm::vec3(0.0f, 1.0f,  0.0f),
		glm::vec3(0.0f, 1.0f,  0.0f),
		glm::vec3(0.0f, 1.0f,  0.0f)
	};

	std::vector<glm::vec2> textures =
	{
		glm::vec2(0.f, 0.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(1.f, 1.f),
		glm::vec2(0.f, 1.f)
	};

	std::vector<unsigned int> indices =
	{
		0, 1, 3,  0, 2, 3
	};
}